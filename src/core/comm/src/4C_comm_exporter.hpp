// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FOUR_C_COMM_EXPORTER_HPP
#define FOUR_C_COMM_EXPORTER_HPP

#include "4C_config.hpp"

#include "4C_comm_pack_helpers.hpp"
#include "4C_comm_utils_factory.hpp"
#include "4C_linalg_map.hpp"

#include <map>
#include <memory>
#include <set>
#include <vector>

FOUR_C_NAMESPACE_OPEN


namespace Core::Communication
{
  /*!
  \brief A class to manage explicit mpi communications

  The discretization management module uses this class to do most of its
  communication. It is used to redistribute grids and to do point-to-point
  communications. It is therefore the only place where explicit calls to MPI
  methods are done.<br>
  It has strong capabilities in gathering and scattering information in a collective AND
  an individual way. Whenever you need explicit communication, check this class first before
  implementing your own mpi stuff.

  */
  class Exporter
  {
    class ExporterHelper;

   public:
    /*!
    \brief Standard Constructor

    this ctor constructs an exporter with no maps. It can than be used to do
    point-to-point communication only, map based exports are not possible!

    \param comm    (in): Communicator that shall be used in exports
    */
    Exporter(MPI_Comm comm);

    /*!
    \brief Standard Constructor

    \param frommap (in): The source map data shall be exported from
    \param tomap   (in): The target map data shall be exported to
    \param comm    (in): Communicator that shall be used in exports
    */
    Exporter(const Core::LinAlg::Map& frommap, const Core::LinAlg::Map& tomap, MPI_Comm comm);

    /*!
    \brief Copy Constructor (default)

    */
    Exporter(const Exporter& old) = default;

    //! @name Access methods

    /*!
    \brief Get communicator
    */
    inline MPI_Comm get_comm() const { return comm_; }

    /*!
    \brief Get source map
    */
    inline const Core::LinAlg::Map& source_map() const { return frommap_; }

    /*!
    \brief Get target map
    */
    inline const Core::LinAlg::Map& target_map() const { return tomap_; }

    //@}


    /*!
     * \name Communication methods according to send and receive plans
     * @{
     */

    /*!
    \brief Communicate a map of objects that implement ParObject

    This method takes a map of objects and redistributes them according to
    the send and receive plans. It is implicitly assumed, that the key in
    the map of objects pointwise matches SourceMap(). It is also assumed
    (and tested), that type T implements the ParObject class.

    \param parobjects (in/out): A map of classes T that implement the
                                class ParObject. On input, the map
                                has a distribution matching SourceMap().
                                On output, the map has a distribution of
                                TargetMap().
    */
    template <typename T>
    void do_export(std::map<int, std::shared_ptr<T>>& parobjects);

    /**
     * @brief Communicate a map of objects that are Packable and Unpackable.
     *
     * This method takes a map of objects and redistributes them according to
     * the send and receive plans. It is implicitly assumed, that the
     * key in the map of objects pointwise matches source_map().
     *
     * @note Since the objects are constructed in-place in @p data, T also needs to be
     * default-initializable.
     */
    template <typename T>
      requires Packable<T> && Unpackable<T> && std::default_initializable<T>
    void do_export(std::map<int, T>& data);

    /**
     * Same as the other function, but for std::unordered_map.
     */
    template <typename T>
      requires Packable<T> && Unpackable<T> && std::default_initializable<T>
    void do_export(std::unordered_map<int, T>& data);

    /*!
    \brief Communicate a map of serial dense matrices

    This method takes a map of serial dense matrices and redistributes them
    according to the send and receive plans. It is implicitly assumed, that the
    key in the map of objects pointwise matches SourceMap().

    \param parobjects (in/out): A map of serial dense matrices. On input, the map
                                has a distribution matching SourceMap().
                                On output, the map has a distribution of
                                TargetMap().
    */
    void do_export(std::map<int, std::shared_ptr<Core::LinAlg::SerialDenseMatrix>>& data);

    /**@}*/
    /*!
     * \name Individual communication methods
     * @{
     */

    /*!
    \brief Send data from one processor to another (nonblocking)

    The method will send an array of chars in a nonblocking way meaning that this method will return
    immediately on the calling processor - even if communication has not finished yet. The char
    array must not be altered or destroyed as long as the communication might still be in progress.
    This can be tested for using Exporter::Wait and the request handle returned. The receiving
    processor should call Exporter::ReceiveAny to receive the message. Note that messages from one
    explicit proc to another explicit proc are non-overtaking meaning they will arrive in the order
    they have been sent.

    \note This is an individual call.

    \param frompid (in)  : sending processors' pid
    \param topid (in)    : target processors' pid
    \param data (in)     : ptr to data to be send
    \param dsize (in)    : size of data (no. of chars)
    \param tag (in)      : tag to be used with message
    \param request (out) : mpi request handle to be used for testing completion of the
                           communication. data may not be altered or destroyed before
                           communication finalized! One can use Exporter::Wait for this.

    \note This is an individual call
    */
    void i_send(const int frompid, const int topid, const char* data, const int dsize,
        const int tag, MPI_Request& request) const;

    /*!
    \brief Send data from one processor to another (nonblocking)

    The method will send an array of ints in a nonblocking way meaning that this method will return
   immediately on the calling processor - even if communication has not finished yet. The int array
   must not be altered or destroyed as long as the communication might still be in progress. This
   can be tested for using Exporter::Wait and the request handle returned. The receiving processor
   should call Exporter::ReceiveAny to receive the message. Note that messages from one explicit
   proc to another explicit proc are non-overtaking meaning they will arrive in the order they have
    been sent.

    \note This is an individual call.

    \param frompid (in)  : sending processors' pid
    \param topid (in)    : target processors' pid
    \param data (in)     : ptr to data to be send
    \param dsize (in)    : size of data (no. of integers)
    \param tag (in)      : tag to be used with message
    \param request (out) : mpi request handle to be used for testing completion of the
                           communication. data may not be altered or destroyed before
                           communication finalized! One can use Exporter::Wait for this.

    \note This is an individual call
    */
    void i_send(const int frompid, const int topid, const int* data, const int dsize, const int tag,
        MPI_Request& request) const;

    /*!
    \brief Send data from one processor to another (nonblocking)

    The method will send an array of doubles in a nonblocking way meaning that this method will
    return immediately on the calling processor - even if communication has not finished yet. The
    double array must not be altered or destroyed as long as the communication might still be in
    progress. This can be tested for using Exporter::Wait and the request handle returned. The
    receiving processor should call Discret::Exporter::ReceiveAny to receive the message. Note that
    messages from one explicit proc to another explicit proc are non-overtaking meaning they will
    arrive in the order they have been sent.

    \note This is an individual call.

    \param frompid (in)  : sending processors' pid
    \param topid (in)    : target processors' pid
    \param data (in)     : ptr to data to be send
    \param dsize (in)    : size of data (no. of doubles)
    \param tag (in)      : tag to be used with message
    \param request (out) : mpi request handle to be used for testing completion of the
                           communication. data may not be altered or destroyed before
                           communication finalized! One can use Exporter::Wait for this.

    \note This is an individual call
    */
    void i_send(const int frompid, const int topid, const double* data, const int dsize,
        const int tag, MPI_Request& request) const;

    /*!
    \brief Receive anything joker (blocking)

    This method receives an MPI_CHAR string message from any source proc with
    any message tag of any length. It simply takes the first message that's
    coming in no matter from which sender of with which tag.
    recvbuff is resized to fit received message.
    the method is blocking for the calling (receiving) proc but for none of the
    other processors.
    It is used together with i_send and Wait to do nonblocking chaotic
    point to point communication.

    \note This is an individual call.

    \warning There is absolutely no guarantee about the order messages are
             received with this method except for one: Messages from the SAME sender
             to the SAME receiver will not overtake each other (which is not a really strong
             statement).

    \param source (output): source the message came from
    \param tag (output): message tag of message received
    \param recvbuff (output): buffer containing received data
    \param length (output): length of message upon receive
    */
    void receive_any(int& source, int& tag, std::vector<char>& recvbuff, int& length) const;
    void receive(const int source, const int tag, std::vector<char>& recvbuff, int& length) const;

    /*!
    \brief Receive anything joker (blocking)

    This method receives an MPI_INT message from any source proc with
    any message tag of any length. It simply takes the first message that's
    coming in no matter from which sender of with which tag.
    recvbuff is resized to fit received message.
    the method is blocking for the calling (receiving) proc but for none of the
    other processors.
    It is used together with i_send and Wait to do nonblocking chaotic
    point to point communication.

    \note This is an individual call.

    \warning There is absolutely no guarantee about the order messages are
             received with this method except for one: Messages from the SAME sender
             to the SAME receiver will not overtake each other (which is not a really strong
             statement).

    \param source (output): source the message came from
    \param tag (output): message tag of message received
    \param recvbuff (output): buffer containing received data
    \param length (output): length of message upon receive
    */
    void receive_any(int& source, int& tag, std::vector<int>& recvbuff, int& length) const;
    void receive(const int source, const int tag, std::vector<int>& recvbuff, int& length) const;

    /*!
    \brief Receive anything joker (blocking)

    This method receives an MPI_DOUBLE message from any source proc with
    any message tag of any length. It simply takes the first message that's
    coming in no matter from which sender of with which tag.
    recvbuff is resized to fit received message.
    the method is blocking for the calling (receiving) proc but for none of the
    other processors.
    It is used together with i_send and Wait to do nonblocking chaotic
    point to point communication.

    \note This is an individual call.

    \warning There is absolutely no guarantee about the order messages are
             received with this method except for one: Messages from the SAME sender
             to the SAME receiver will not overtake each other (which is not a really strong
             statement).

    \param source (output): source the message came from
    \param tag (output): message tag of message received
    \param recvbuff (output): buffer containing received data
    \param length (output): length of message upon receive
    */
    void receive_any(int& source, int& tag, std::vector<double>& recvbuff, int& length) const;
    void receive(const int source, const int tag, std::vector<double>& recvbuff, int& length) const;

    /**@}*/

    /*!
    \brief wait for nonblocking send to finish

    The method is used together with Isend and ReceiveAny to guarantee finalization
    of a communication. It is an individual call done by the sending processor to guarantee
    that message was taken from the sendbuffer before destroying the sendbuffer.
    This method is blocking and will return one communication associated with request has
    left the sender.

    \param request (in): mpi request handle

    */
    void wait(MPI_Request& request) const
    {
      MPI_Status status;
      MPI_Wait(&request, &status);
    }

    /*!
     * \name Collective communication methods
     * @{
     */

    /*!
    \brief performs an allreduce operation on all processors
         and sends the result to all processors

    \param sendbuff (input): buffer containing data that has to be sent
    \param recvbuff (output): buffer containing received data
    \param mpi_op   (input): MPI operation
    */
    void allreduce(std::vector<int>& sendbuff, std::vector<int>& recvbuff, MPI_Op mpi_op);


    /*!
     * \brief Send data from one processor to all other processors (blocking)
     *
     * The method will send an array of chars from one sender (rank==frompid) to all other procs.
     *
     * All processors have to call this method (no matter whether the proc is sender or receiver).
     * The sending proc (rank==frompid) has to provide the data vector. The size of the vector
     * at the receivers (rank!=frompid) is automatically adjusted.
     *
     * \param frompid Id of the root processor that is the only sender
     * \param data vector of the data (must only be filled by the sender)
     * \param tag mpi tag
     */
    void broadcast(int frompid, std::vector<char>& data, int tag) const;

    //@}

   private:
    /*!
    \brief Do initialization of the exporter
    */
    void construct_exporter();

    /*!
    \brief Get PID
    */
    inline int my_pid() const { return myrank_; }
    /*!
    \brief Get no. of processors
    */
    inline int num_proc() const { return numproc_; }

    /*!
    \brief Get sendplan_
    */
    inline std::vector<std::set<int>>& send_plan() { return sendplan_; }

    /*!
    \brief generic export algorithm that delegates the specific pack/unpack to a helper
     */
    void generic_export(ExporterHelper& helper);

   private:
    //! dummy map in case of empty exporter
    Core::LinAlg::Map dummymap_;
    //! source layout
    const Core::LinAlg::Map& frommap_;
    //! target map
    const Core::LinAlg::Map& tomap_;
    //! communicator
    MPI_Comm comm_;
    //! PID
    int myrank_;
    //! no. of processors
    int numproc_;
    //! sending information
    std::vector<std::set<int>> sendplan_;

    /// Internal helper class for Exporter that encapsulates packing and unpacking
    /*!
      The communication algorithm we use to export a map of objects is the same
      independent of the actual type of objects we have. However, different
      object types require different packing and unpacking routines. So we put
      the type specific stuff in a helper class and get away with one clean
      communication algorithm. Nice.
     */
    class ExporterHelper
    {
     public:
      /// have a virtual destructor
      virtual ~ExporterHelper() = default;

      /// Pack one object
      /*!
        Get the object by gid, pack it and append it to the sendblock. We only
        pack it if we know about it.
       */
      virtual bool pack_object(int gid, PackBuffer& sendblock) = 0;

      /// Unpack one object
      /*!
        After receiving we know the gid of the object and have its packed data
        at position index in recvblock. index must be incremented by the objects
        size.
       */
      virtual void unpack_object(int gid, UnpackBuffer& buffer) = 0;

      /// after communication remove all objects that are not in the target map
      virtual void post_export_cleanup(Exporter* exporter) = 0;
    };


    /// Concrete helper class that handles std::shared_ptrs to ParObjects
    template <typename T>
      requires std::derived_from<T, ParObject>
    class ParObjectExporterHelper : public ExporterHelper
    {
     public:
      explicit ParObjectExporterHelper(std::map<int, std::shared_ptr<T>>& parobjects)
          : parobjects_(parobjects)
      {
      }

      bool pack_object(int gid, PackBuffer& sendblock) override
      {
        typename std::map<int, std::shared_ptr<T>>::const_iterator curr = parobjects_.find(gid);
        if (curr != parobjects_.end())
        {
          add_to_pack(sendblock, *curr->second);
          return true;
        }
        return false;
      }

      void unpack_object(int gid, UnpackBuffer& buffer) override
      {
        ParObject* o = factory(buffer);
        T* ptr = dynamic_cast<T*>(o);
        if (!ptr)
          FOUR_C_THROW("typename T in template does not implement ParObject (dynamic_cast failed)");
        std::shared_ptr<T> refptr(ptr);
        // add object to my map
        parobjects_[gid] = refptr;
      }

      void post_export_cleanup(Exporter* exporter) override
      {
        // loop map and kick out everything that's not in TargetMap()
        std::map<int, std::shared_ptr<T>> newmap;
        typename std::map<int, std::shared_ptr<T>>::const_iterator fool;
        for (fool = parobjects_.begin(); fool != parobjects_.end(); ++fool)
          if (exporter->target_map().my_gid(fool->first)) newmap[fool->first] = fool->second;
        swap(newmap, parobjects_);
      }

     private:
      std::map<int, std::shared_ptr<T>>& parobjects_;
    };


    /// Concrete helper class that handles std::shared_ptrs to any object
    /**!
       The objects considered here must have a default constructor and must be
       supported by add_to_pack and extract_from_pack
       functions.

       Ideally one would manage ParObject with this helper as well, however, the
       cast to the concrete ParObject type prevents that.
     */
    template <typename T>
      requires std::default_initializable<T> && Packable<T> && Unpackable<T>
    class AnyObjectExporterHelper : public ExporterHelper
    {
     public:
      explicit AnyObjectExporterHelper(std::map<int, std::shared_ptr<T>>& objects)
          : objects_(objects)
      {
      }

      bool pack_object(int gid, PackBuffer& sendblock) override
      {
        typename std::map<int, std::shared_ptr<T>>::const_iterator curr = objects_.find(gid);
        if (curr != objects_.end())
        {
          add_to_pack(sendblock, *curr->second);
          return true;
        }
        return false;
      }

      void unpack_object(int gid, UnpackBuffer& buffer) override
      {
        std::shared_ptr<T> obj = std::make_shared<T>();
        extract_from_pack(buffer, *obj);

        // add object to my map
        objects_[gid] = obj;
      }

      void post_export_cleanup(Exporter* exporter) override
      {
        // loop map and kick out everything that's not in TargetMap()
        std::map<int, std::shared_ptr<T>> newmap;
        typename std::map<int, std::shared_ptr<T>>::const_iterator fool;
        for (fool = objects_.begin(); fool != objects_.end(); ++fool)
          if (exporter->target_map().my_gid(fool->first)) newmap[fool->first] = fool->second;
        swap(newmap, objects_);
      }

     private:
      std::map<int, std::shared_ptr<T>>& objects_;
    };



    /**
     * Export anything that is default-initializable, Packable and Unpackable.
     */
    template <template <typename, typename, typename...> typename MapType, typename T,
        typename... Args>
      requires std::default_initializable<T> && Packable<T> && Unpackable<T>
    class DefaultExporterHelper : public ExporterHelper
    {
     public:
      using Map = MapType<int, T, Args...>;

      explicit DefaultExporterHelper(Map& objects) : objects_(objects) {}

      bool pack_object(int gid, PackBuffer& sendblock) override
      {
        auto curr = objects_.find(gid);
        if (curr != objects_.end())
        {
          add_to_pack(sendblock, curr->second);
          return true;
        }
        return false;
      }

      void unpack_object(int gid, UnpackBuffer& buffer) override
      {
        extract_from_pack(buffer, objects_[gid]);
      }

      void post_export_cleanup(Exporter* exporter) override
      {
        // loop map and kick out everything that's not in TargetMap()
        Map newmap;
        for (const auto& [id, obj] : objects_)
          if (exporter->target_map().my_gid(id)) newmap[id] = obj;
        swap(newmap, objects_);
      }

     private:
      Map& objects_;
    };
  };
}  // namespace Core::Communication


template <typename T>
void Core::Communication::Exporter::do_export(std::map<int, std::shared_ptr<T>>& parobjects)
{
  ParObjectExporterHelper<T> helper(parobjects);
  generic_export(helper);
}

template <typename T>
  requires Core::Communication::Packable<T> && Core::Communication::Unpackable<T> &&
           std::default_initializable<T>
void Core::Communication::Exporter::do_export(std::map<int, T>& data)
{
  DefaultExporterHelper helper(data);
  generic_export(helper);
}

template <typename T>
  requires Core::Communication::Packable<T> && Core::Communication::Unpackable<T> &&
           std::default_initializable<T>
void Core::Communication::Exporter::do_export(std::unordered_map<int, T>& data)
{
  DefaultExporterHelper helper(data);
  generic_export(helper);
}


FOUR_C_NAMESPACE_CLOSE

#endif
