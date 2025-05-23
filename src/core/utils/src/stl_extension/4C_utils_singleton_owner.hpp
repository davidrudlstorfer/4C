// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FOUR_C_UTILS_SINGLETON_OWNER_HPP
#define FOUR_C_UTILS_SINGLETON_OWNER_HPP

#include "4C_config.hpp"

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <type_traits>

FOUR_C_NAMESPACE_OPEN

namespace Core::Utils
{
  //! Action types for the singleton owner
  enum class SingletonAction
  {
    create,   //!< Create an instance (if not already created)
    destruct  //!< Destruct an existing instance
  };

  /**
   * @brief Registry for SingletonOwner objects.
   *
   * This class solves a problem with singleton objects. Before the program starts, we call
   * SingletonOwnerRegistry::initialize() to ensure that the SingletonOwnerRegistry is initialized
   * before any SingletonOwner objects are created. At the end of the program, we call
   * SingletonOwnerRegistry::finalize() which will destroy all singletons that are managed by any
   * SingletonOwner. While this might seem pointless, given that the program is about to end, it is
   * necessary to ensure that resources held within the singletons are released properly. Certain
   * libraries (e.g. Kokkos) perform clean-up tasks at the end of the program, which might fail if
   * associated resources were still held by singletons.
   *
   * @note Funnily enough, this class is a singleton itself.
   */
  class SingletonOwnerRegistry
  {
   public:
    /**
     * RAII class to ensure that finalize() is called at the end of scope.
     */
    class [[nodiscard]] ScopeGuard
    {
     public:
      ScopeGuard() { SingletonOwnerRegistry::initialize(); }
      ~ScopeGuard() { SingletonOwnerRegistry::finalize(); }

      // Delete special members.
      ScopeGuard(const ScopeGuard&) = delete;
      ScopeGuard(ScopeGuard&&) = delete;
      ScopeGuard& operator=(const ScopeGuard&) = delete;
      ScopeGuard& operator=(ScopeGuard&&) = delete;
    };

    /**
     * @brief Initialize the SingletonOwnerRegistry.
     *
     * This function is called at the beginning of the program to ensure that the
     * SingletonOwnerRegistry is initialized before any SingletonOwner objects are created. This is
     * necessary to ensure that the SingletonOwnerRegistry is destroyed after all SingletonOwner
     * objects are destroyed.
     *
     * @note Prefer to use the ScopeGuard class to ensure that finalize() is called at the end of
     * scope.
     *
     */
    static void initialize();

    /**
     * @brief Destroy all singletons that are managed by a SingletonOwner.
     *
     * This function is called at the end of the program to ensure that all singletons are
     * destructed.
     *
     * @note Prefer to use the ScopeGuard class which calls this function at the end of scope.
     */
    static void finalize();

   private:
    SingletonOwnerRegistry() = default;
    static SingletonOwnerRegistry& instance();

    /**
     * Register a deleter function for a singleton owner.
     */
    static void register_deleter(void* owner, std::function<void()> deleter);

    /**
     * Unregister a deleter function for a singleton owner.
     */
    static void unregister(void* owner);

    /**
     * Store the deleters.
     */
    std::map<void*, std::function<void()>> deleters_;

    template <typename T, typename... CreationArgs>
    friend class SingletonOwner;
  };

  /*!
   * @brief Owner class for singletons
   *
   * Manages (creates, destructs, or returns) the singleton instance of any type.
   *
   * @tparam T  Type of the singleton
   * @tparam creation_args_ A template parameter pack which contains the types of all arguments that
   *   are necessary to create the owned singleton object.
   */
  template <typename T, typename... CreationArgs>
  class SingletonOwner
  {
   public:
    /*!
     * @brief Constructor.
     *
     * Construction is preferably done via make_singleton_owner() which comes with powerful template
     * argument deduction.
     *
     * @param [in] creator  Function (object) that can create the singleton instance and return a
     * unique_ptr to it. The function's signature must be `std::unique_ptr<T>(creation_args_...)`.
     */
    template <typename Fn>
    SingletonOwner(Fn&& creator)
      requires std::is_invocable_r_v<std::unique_ptr<T>, Fn, CreationArgs...>;

    //! Deleted copy constructor. Only one object can own the singleton.
    SingletonOwner(const SingletonOwner& other) = delete;

    //! Deleted move constructor to keep things simple.
    SingletonOwner(SingletonOwner&& other) noexcept = delete;

    //! Deleted copy assignment. Only one object can own the singleton.
    SingletonOwner& operator=(const SingletonOwner& other) = delete;

    //! Deleted move assignment to keep things simple.
    SingletonOwner& operator=(SingletonOwner&& other) noexcept = delete;

    //! Destructor.
    ~SingletonOwner();

    /*!
     * @brief Return pointer to singleton instance
     *
     * If the @p action is SingletonAction::create, create the singleton instance (if not already
     * created) and return the pointer to it. If the @p action is SingletonAction::destruct,
     * destroy the instance and return a `nullptr`.
     *
     * In case any parameters are needed to call the constructor of the singleton object, they have
     * to be passed as well. Unfortunately, this is necessary regardless of the @p action flag.
     *
     */
    T* instance(SingletonAction action, CreationArgs... args);

    /**
     * @brief Destroy the singleton instance.
     */
    void destroy_instance();

   private:
    //! singleton instance
    std::unique_ptr<T> instance_;

    //! Function that creates a singleton object
    std::function<std::unique_ptr<T>(CreationArgs...)> creator_;
  };

  /**
   * @brief Store multiple SingletonOwner objects by a given Key.
   *
   * This class is a useful extension to SingletonOwner in places where multiple differently
   * parametrized objects of the same Singleton class should be used. This classes instance() method
   * takes an additional Key argument to distinguish between the singletons and apart from this
   * difference behaves the same as SingletonOwner::instance().
   *
   * @tparam Key The type used to access the different instances.
   * @tparam T The type of the singleton.
   * @tparam creation_args_ A template parameter pack which contains the types of all arguments that
   *   are necessary to create the owned singleton object.
   */
  template <typename Key, typename T, typename... CreationArgs>
  class SingletonMap
  {
   public:
    /*!
     * @brief Constructor
     *
     * Construction is preferably done via make_singleton_map() which comes with powerful template
     * argument deduction.
     *
     * @param [in] creator  Function (object) that can create the singleton instance and return a
     * unique_ptr to it. The function's signature must be `std::unique_ptr<T>(creation_args_...)`.
     */
    template <typename Fn>
    SingletonMap(Fn&& creator)
      requires std::is_invocable_r_v<std::unique_ptr<T>, Fn, CreationArgs...>;

    /**
     * Return a SingletonOwner for the given @p key. If it does not exist, one is created the first
     * time this function is called with the @p key. This call is normally directly followed by a
     * call to SingletonOwner::instance() e.g.
     *
     * @code
     *   singleton_map[disname].instance(Core::Utils::SingletonAction::create, numdofpernode,
     * disname);
     * @endoce
     */
    SingletonOwner<T, CreationArgs...>& operator[](const Key& key);

   private:
    //! Function that creates a singleton object
    std::function<std::unique_ptr<T>(CreationArgs...)> creator_;

    //! All SingletonOwner objects that are stored internally.
    std::map<Key, SingletonOwner<T, CreationArgs...>> map_;
  };


  /**
   * Convenience function to create a SingletonOwner object. This function mainly exists to deduce
   * the template arguments of more complicated @p creator functions and pass them on to
   * SingletonOwner. This allows to write code like:
   *
   * @code
   *   // static initialization upon first call
   *   static auto singleton_owner = make_singleton_owner(
   *     [](double a, int b) { return std::make_unique<MyClass>(a,b); });
   *
   *   // access an instance constructed from given parameters (if it does not exist already)
   *   singleton_owner.Instance(SingletonAction::create, 1.0, 2);
   * @endcode
   *
   * @param creator Function (object) that can create the singleton instance and return a
   *   unique_ptr to it. The function's signature must be `std::unique_ptr<T>(creation_args_...)`
   * @return A SingletonOwner object that should probably be stored as a static variable.
   */
  template <typename Fn>
  auto make_singleton_owner(Fn&& creator);

  /**
   * Similar to make_singleton_owner(), this function helps to deduce the template arguments for a
   * SingletonMap. Note that you need to specify the first template argument, which defines the type
   * of the key that is used to access the map entries.
   *
   * @code
   *
   *   // static initialization upon first call
   *  static auto singleton_map = ::Utils::make_singleton_map<std::string>(
   *    [](const int numdofpernode, const int numscal, const std::string& disname)
   *    {
   *      return std::unique_ptr<ScaTraEleBoundaryCalcElchElectrodeSTIThermo<distype>>(
   *         new ScaTraEleBoundaryCalcElchElectrodeSTIThermo<distype>(
   *           numdofpernode, numscal, disname));
   *    });
   *
   *  // Use the map as
   *  singleton_map[disname].Instance(SingletonAction::create, numdofpernode, numscal, disname);
   *
   * @endcode
   *
   * @param creator Function (object) that can create the singleton instance and return a
   *   unique_ptr to it. The function's signature must be `std::unique_ptr<T>(creation_args_...)`
   * @return A SingletonMap object that should probably be stored as a static variable.
   */
  template <typename KeyType, typename Fn>
  auto make_singleton_map(Fn&& creator);



  // --- template and inline functions --- //


  template <typename T, typename... CreationArgs>
  template <typename Fn>
  SingletonOwner<T, CreationArgs...>::SingletonOwner(Fn&& creator)
    requires std::is_invocable_r_v<std::unique_ptr<T>, Fn, CreationArgs...>
      : creator_(std::forward<Fn>(creator))
  {
    SingletonOwnerRegistry::register_deleter(this, [this]() { destroy_instance(); });
  }

  template <typename T, typename... CreationArgs>
  SingletonOwner<T, CreationArgs...>::~SingletonOwner()
  {
    SingletonOwnerRegistry::unregister(this);
  }


  template <typename T, typename... CreationArgs>
  T* SingletonOwner<T, CreationArgs...>::instance(SingletonAction action, CreationArgs... args)
  {
    if (action == SingletonAction::create and !instance_)
    {
      instance_ = creator_(args...);
    }
    else if (action == SingletonAction::destruct)
    {
      destroy_instance();
    }
    return instance_.get();
  }


  template <typename T, typename... CreationArgs>
  void SingletonOwner<T, CreationArgs...>::destroy_instance()
  {
    instance_.reset();
  }


  template <typename Key, typename T, typename... CreationArgs>
  template <typename Fn>
  SingletonMap<Key, T, CreationArgs...>::SingletonMap(Fn&& creator)
    requires std::is_invocable_r_v<std::unique_ptr<T>, Fn, CreationArgs...>
      : creator_(std::forward<Fn>(creator))
  {
  }


  template <typename Key, typename T, typename... CreationArgs>
  SingletonOwner<T, CreationArgs...>& SingletonMap<Key, T, CreationArgs...>::operator[](
      const Key& key)
  {
    auto it = map_.find(key);
    if (it == map_.end())
    {
      it = map_.emplace(key, creator_).first;
    }

    return it->second;
  }


  /**
   * Implementation details.
   */
  namespace detail  // NOLINT
  {
    template <class F>
    struct DeduceSingletonOwner
    {
    };

    template <class Ret, class C, class... Args>
    struct DeduceSingletonOwner<Ret (C::*)(Args...)>
    {
      using ReturnType = typename Ret::element_type;
      using type = SingletonOwner<ReturnType, Args...>;
    };

    template <class Ret, class C, class... Args>
    struct DeduceSingletonOwner<Ret (C::*)(Args...) const>
    {
      using ReturnType = typename Ret::element_type;
      using type = SingletonOwner<ReturnType, Args...>;
    };

    template <typename KeyType, class F>
    struct DeduceSingletonMap
    {
    };

    template <typename KeyType, class Ret, class C, class... Args>
    struct DeduceSingletonMap<KeyType, Ret (C::*)(Args...)>
    {
      using ReturnType = typename Ret::element_type;
      using type = SingletonMap<KeyType, ReturnType, Args...>;
    };

    template <typename KeyType, class Ret, class C, class... Args>
    struct DeduceSingletonMap<KeyType, Ret (C::*)(Args...) const>
    {
      using ReturnType = typename Ret::element_type;
      using type = SingletonMap<KeyType, ReturnType, Args...>;
    };

  }  // namespace detail

  template <typename Fn>
  auto make_singleton_owner(Fn&& creator)
  {
    using T = typename detail::DeduceSingletonOwner<decltype(&Fn::operator())>::type;
    return T(std::forward<Fn>(creator));
  }


  template <typename KeyType, typename Fn>
  auto make_singleton_map(Fn&& creator)
  {
    using T = typename detail::DeduceSingletonMap<KeyType, decltype(&Fn::operator())>::type;
    return T(std::forward<Fn>(creator));
  }
}  // namespace Core::Utils


FOUR_C_NAMESPACE_CLOSE

#endif
