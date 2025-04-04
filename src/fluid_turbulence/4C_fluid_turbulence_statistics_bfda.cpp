// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "4C_fluid_turbulence_statistics_bfda.hpp"

#include "4C_comm_exporter.hpp"
#include "4C_fem_general_element.hpp"
#include "4C_fem_general_node.hpp"

#include <Teuchos_ParameterList.hpp>

#include <fstream>

FOUR_C_NAMESPACE_OPEN


/*----------------------------------------------------------------------*/
/*!
  \brief Standard Constructor (public)

    o Create vector "zcoordinates" with coordinates along z-axis
  o Create matrix "rcoordinates_" with coordinates of radial coordinates of evaluation planes
    columns are evaluation planes corresponding to the positions in "posEvaluation_"

*/
/*----------------------------------------------------------------------*/
FLD::TurbulenceStatisticsBfda::TurbulenceStatisticsBfda(
    std::shared_ptr<Core::FE::Discretization> actdis, Teuchos::ParameterList& params,
    const std::string& statistics_outfilename)
    : discret_(actdis), params_(params), statistics_outfilename_(statistics_outfilename)
{
  if (Core::Communication::my_mpi_rank(discret_->get_comm()) == 0)
  {
    std::cout << "This is the turbulence statistics manager of a benchmark of blood flowing "
                 "through a channel:"
              << std::endl;
  }

  /*---------------------------------------------------------------------*/
  /*---------------------------------------------------------------------*/
  // Parameters:
  // z-coordinates for evaluation
  pos_evaluation_.clear();
  pos_evaluation_.push_back(-0.088);
  pos_evaluation_.push_back(-0.064);
  pos_evaluation_.push_back(-0.048);
  pos_evaluation_.push_back(-0.02);
  pos_evaluation_.push_back(-0.008);
  pos_evaluation_.push_back(0.0);
  pos_evaluation_.push_back(0.008);
  pos_evaluation_.push_back(0.016);
  pos_evaluation_.push_back(0.024);
  pos_evaluation_.push_back(0.032);
  pos_evaluation_.push_back(0.06);
  pos_evaluation_.push_back(0.08);
  /*---------------------------------------------------------------------*/
  /*---------------------------------------------------------------------*/

  // number of evaluation planes
  int numPosEvaluation = pos_evaluation_.size();

  // Vector for evaluation planes of actual mesh (nodes not exactly on evaluation planes
  act_pos_evaluation_.clear();

  // Resize matrix for radial coordinates in a first step (final size is later set)
  rcoordinates_.reshape(1, numPosEvaluation);


  //----------------------------------------------------------------------
  // plausibility check
  int numdim = params_.get<int>("number of velocity degrees of freedom");
  if (numdim != 3) FOUR_C_THROW("Evaluation of turbulence statistics only for 3d flow problems!");

  //----------------------------------------------------------------------
  // allocate some (toggle) vectors
  const Core::LinAlg::Map* dofrowmap = discret_->dof_row_map();
  togglew_ = Core::LinAlg::create_vector(*dofrowmap, true);
  togglep_ = Core::LinAlg::create_vector(*dofrowmap, true);


  //----------------------------------------------------------------------
  // create sets of coordinates
  //----------------------------------------------------------------------
  // the criterion allows differences in coordinates by 1e-9
  std::set<double, LineSortCriterion> zavcoords;
  std::set<double, LineSortCriterion> ravcoords;


  // loop nodes and build sets of lines in z-direction
  // accessible on this proc
  for (int i = 0; i < discret_->num_my_row_nodes(); ++i)
  {
    // create node object
    Core::Nodes::Node* node = discret_->l_row_node(i);

    // Is the actual node on z-axis?
    // => get z-coordinates of nodes on z-axis
    if (node->x()[1] < 2e-9 && node->x()[1] > -2e-9 && node->x()[0] < 2e-9 && node->x()[0] > -2e-9)
      // then insert z-coordinate to vector "zavcoords"
      zavcoords.insert(node->x()[2]);
  }

  //--------------------------------------------------------------------
  // round robin loop to communicate coordinates to all procs
  //--------------------------------------------------------------------
  int myrank = Core::Communication::my_mpi_rank(discret_->get_comm());
  int numprocs = Core::Communication::num_mpi_ranks(discret_->get_comm());

  std::vector<char> sblock;
  std::vector<char> rblock;

  // create an exporter for point to point communication
  Core::Communication::Exporter exporter(discret_->get_comm());

  // first, communicate coordinates in x1-direction
  for (int np = 0; np < numprocs; ++np)
  {
    Core::Communication::PackBuffer data;

    for (std::set<double, LineSortCriterion>::iterator zline = zavcoords.begin();
        zline != zavcoords.end(); ++zline)
    {
      add_to_pack(data, *zline);
    }
    std::swap(sblock, data());

    MPI_Request request;
    int tag = myrank;

    int frompid = myrank;
    int topid = (myrank + 1) % numprocs;

    int length = sblock.size();

    exporter.i_send(frompid, topid, sblock.data(), sblock.size(), tag, request);

    rblock.clear();

    // receive from predecessor
    frompid = (myrank + numprocs - 1) % numprocs;
    exporter.receive_any(frompid, tag, rblock, length);

    if (tag != (myrank + numprocs - 1) % numprocs)
    {
      FOUR_C_THROW("received wrong message (ReceiveAny)");
    }

    exporter.wait(request);

    {
      // for safety
      Core::Communication::barrier(exporter.get_comm());
    }

    //--------------------------------------------------
    // Unpack received block into set of all planes.
    {
      std::vector<double> coordsvec;

      coordsvec.clear();

      Core::Communication::UnpackBuffer buffer(rblock);
      while (!buffer.at_end())
      {
        double onecoord;
        extract_from_pack(buffer, onecoord);
        zavcoords.insert(onecoord);
      }
    }
  }



  //----------------------------------------------------------------------
  // push coordinates in vectors (copy zavcoords to a new vector zcoordinates)
  //----------------------------------------------------------------------
  //  {
  zcoordinates_ = std::make_shared<std::vector<double>>();

  for (std::set<double, LineSortCriterion>::iterator coord1 = zavcoords.begin();
      coord1 != zavcoords.end(); ++coord1)
  {
    zcoordinates_->push_back(*coord1);
  }
  //  }



  //----------------------------------------------------------------------
  // number of coordinates in z-direction
  //----------------------------------------------------------------------
  numzcoor_ = zcoordinates_->size();



  //----------------------------------------------------------------------
  // Error Message if no nodes are on z-axis (nothing for evaluation)
  //----------------------------------------------------------------------
  if (Core::Communication::my_mpi_rank(discret_->get_comm()) == 0)
  {
    if (numzcoor_ == 0)
    {
      std::cout << std::endl << "WARNING!" << std::endl;
      std::cout << "No nodes are on the centerline! Please check if center of cells is on the "
                   "z-axis so that there are no nodes on it!"
                << std::endl
                << std::endl;
    }
    else
      std::cout << "Number of nodes on z-axis: " << numzcoor_ << std::endl;
  }



  // loop nodes and build sets of lines in radial direction
  // not exact position of evaluation is evaluated because there could be no nodes
  // instead the next higher z-coordinate is used if the exact position is not available
  // accessible on this proc

  // Is the actual node on y-axis of an evaluation plane?
  // => get radial coordinates of nodes on evaluation planes
  // Loop through all evaluation planes
  for (int actPosEval = 0; actPosEval < numPosEvaluation; actPosEval++)
  {
    // Reset variable for radial coordinates on each proc
    ravcoords.clear();


    int actRadNode = 0;

    // get exact or next z coordinate next to the evaluation plane where a node exists
    // therefore loop through the entries in zcoordinates_ until an entry is equal or bigger
    // than the desired position of the evaluation plane
    double actZ = 0;
    for (std::set<double, LineSortCriterion>::iterator temp_ptr_actZ = zavcoords.begin();
        temp_ptr_actZ != zavcoords.end(); ++temp_ptr_actZ)
    {
      if (*temp_ptr_actZ >= pos_evaluation_[actPosEval])
      {
        actZ = *temp_ptr_actZ;
        act_pos_evaluation_.push_back(*temp_ptr_actZ);
        break;
      }
    }

    // Loop to get all radial coordinates of this evaluation plane
    for (int i = 0; i < discret_->num_my_row_nodes(); ++i)
    {
      // create node object
      Core::Nodes::Node* node = discret_->l_row_node(i);

      // Get all nodes on evaluation plane at this z position with y=0 and x>2e-9
      if (node->x()[2] < actZ + 2e-9 && node->x()[2] > actZ - 2e-9 && node->x()[1] < 2e-9 &&
          node->x()[1] > -2e-9 && node->x()[0] > 2e-9)
      {
        // insert radial coordinate (here x) to matrix
        // rcoordinates_(actRadNode, actPosEval)=1.0; //(*node).X()[0];
        // ///////////////////////////////////////////////////////////////////////////////////////////
        ravcoords.insert((*node).x()[0]);
        actRadNode++;
      }
    }


    int countActRadNodeOnAllProcs = 0;
    Core::Communication::sum_all(&actRadNode, &countActRadNodeOnAllProcs, 1, discret_->get_comm());

    int myrank = Core::Communication::my_mpi_rank(discret_->get_comm());
    int numprocs = Core::Communication::num_mpi_ranks(discret_->get_comm());

    std::vector<char> sblock;
    std::vector<char> rblock;

    // create an exporter for point to point communication
    Core::Communication::Exporter exporter(discret_->get_comm());

    // first, communicate coordinates in x1-direction
    for (int np = 0; np < numprocs; ++np)
    {
      Core::Communication::PackBuffer data;

      for (std::set<double, LineSortCriterion>::iterator zline = ravcoords.begin();
          zline != ravcoords.end(); ++zline)
      {
        add_to_pack(data, *zline);
      }
      std::swap(sblock, data());

      MPI_Request request;
      int tag = myrank;

      int frompid = myrank;
      int topid = (myrank + 1) % numprocs;

      int length = sblock.size();

      exporter.i_send(frompid, topid, sblock.data(), sblock.size(), tag, request);

      rblock.clear();

      // receive from predecessor
      frompid = (myrank + numprocs - 1) % numprocs;
      exporter.receive_any(frompid, tag, rblock, length);

      if (tag != (myrank + numprocs - 1) % numprocs)
      {
        FOUR_C_THROW("received wrong message (ReceiveAny)");
      }

      exporter.wait(request);

      {
        // for safety
        Core::Communication::barrier(exporter.get_comm());
      }

      //--------------------------------------------------
      // Unpack received block into set of all planes.
      {
        std::vector<double> coordsvec;

        coordsvec.clear();
        //                ravcoords.clear();
        //                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Core::Communication::UnpackBuffer buffer(rblock);
        while (!buffer.at_end())
        {
          double onecoord;
          extract_from_pack(buffer, onecoord);
          ravcoords.insert(onecoord);
        }
      }
    }


    // Is actual number of radial coordinates higher than current maximum (= number of rows of
    // matrix) +1 at actRadNode because counting starts at 0 but numRows starts counting at 1
    if ((countActRadNodeOnAllProcs + 1) > rcoordinates_.numRows())
      rcoordinates_.reshape(countActRadNodeOnAllProcs + 1, numPosEvaluation);


    //----------------------------------------------------------------------
    // push ravcoords in matrix rcoordinates_
    //----------------------------------------------------------------------
    int count_coord1 = 1;
    for (std::set<double, LineSortCriterion>::iterator coord1 = ravcoords.begin();
        coord1 != ravcoords.end(); ++coord1)
    {
      rcoordinates_(count_coord1, actPosEval) = *coord1;
      count_coord1++;
    }
  }  // End of Loop for evaluation planes

  // Get maximum number of radial points
  numrstatlocations_ = rcoordinates_.numRows();

  //----------------------------------------------------------------------
  // Error Message if no nodes are in radial direction in a specific evaluation plane (nothing for
  // evaluation)
  //----------------------------------------------------------------------
  if (Core::Communication::my_mpi_rank(discret_->get_comm()) == 0)
  {
    if (numrstatlocations_ == 0)
    {
      std::cout << std::endl << "WARNING!" << std::endl;
      std::cout << "No nodes are on the evaluation planes! Please check if center of cells is on "
                   "the z-axis so that there are no nodes on it!"
                << std::endl
                << std::endl;
    }
  }

  //----------------------------------------------------------------------
  // allocate arrays for sums of mean values
  //----------------------------------------------------------------------
  zsumw_ = std::make_shared<Core::LinAlg::SerialDenseMatrix>();
  zsumw_->reshape(1, numzcoor_);

  zsump_ = std::make_shared<Core::LinAlg::SerialDenseMatrix>();
  zsump_->reshape(1, numzcoor_);

  rsumw_ = std::make_shared<Core::LinAlg::SerialDenseMatrix>();
  rsumw_->reshape(numrstatlocations_, numPosEvaluation);

  rsump_ = std::make_shared<Core::LinAlg::SerialDenseMatrix>();
  rsump_->reshape(numrstatlocations_, numPosEvaluation);


  // set number of samples to zero
  numsamp_ = 0;

  //----------------------------------------------------------------------
  // initialize output and initially open respective statistics output file

  std::shared_ptr<std::ofstream> log;
  std::shared_ptr<std::ofstream> log_2;

  if (Core::Communication::my_mpi_rank(discret_->get_comm()) == 0)
  {
    std::string s(statistics_outfilename_);
    std::string s2(statistics_outfilename_);

    //    s.append(".flow_statistics");
    s.append(".flow_statistics_along_z");
    s2.append(".flow_statistics_evaluation_planes");

    log = std::make_shared<std::ofstream>(s.c_str(), std::ios::out);
    (*log)
        << "# Statistics for turbulent incompressible flow of blood flowing through a channel\n\n";

    log_2 = std::make_shared<std::ofstream>(s2.c_str(), std::ios::out);
    (*log_2)
        << "# Statistics for turbulent incompressible flow of blood flowing through a channel\n\n";

    log->flush();
    log_2->flush();
  }

  return;
}  // TurbulenceStatisticsBfda::TurbulenceStatisticsBfda



void FLD::TurbulenceStatisticsBfda::do_time_sample(Core::LinAlg::Vector<double>& velnp)
{
  //----------------------------------------------------------------------
  // increase sample counter
  //----------------------------------------------------------------------
  numsamp_++;

  //----------------------------------------------------------------------
  //----------------------------------------------------------------------
  // sampling of velocity/pressure values along z axis
  //----------------------------------------------------------------------
  //----------------------------------------------------------------------
  int znodnum = -1;

  // Big loop along z-axis
  for (std::vector<double>::iterator zline = zcoordinates_->begin(); zline != zcoordinates_->end();
      ++zline)
  {
    znodnum++;

    // toggle vectors are one in the position of a dof of this node, else 0
    togglew_->put_scalar(0.0);
    togglep_->put_scalar(0.0);

    // count the number of nodes in x3-direction contributing to this nodal value
    int countnodes = 0;

    // write a 1.0 at the position of the actual node of the processor in the toggle vectors
    for (int nn = 0; nn < discret_->num_my_row_nodes(); ++nn)
    {
      Core::Nodes::Node* node = discret_->l_row_node(nn);

      // If node is on z-axis then get toggle vector for pressure and velocity at actual node //
      // this is the wall node
      if ((node->x()[2] < (*zline + 2e-9) and node->x()[2] > (*zline - 2e-9)) and
          (node->x()[0] < (2e-9) and node->x()[0] > (-2e-9)) and
          (node->x()[1] < (2e-9) and node->x()[1] > (-2e-9)))
      {
        // Creation of vector dof that stores the values that variable "discret" points to
        // dof = [position of u     position of v     position of w     position of p]
        // position in terms of position of all degrees of freedom: u1 v1 w1 p1 u2 v2 w2 p2 u3 ...
        std::vector<int> dof = discret_->dof(node);

        double one = 1.0;
        togglep_->replace_global_values(1, &one, &(dof[3]));
        togglew_->replace_global_values(1, &one, &(dof[2]));


        // increase counter for evaluated nodes
        countnodes++;
      }
    }

    // Count nodes on all procs (should be 1)
    int countnodesonallprocs = 1;
    Core::Communication::sum_all(&countnodes, &countnodesonallprocs, 1, discret_->get_comm());
    if (countnodesonallprocs)
    {
      //----------------------------------------------------------------------
      // get values for velocity derivative and pressure
      //----------------------------------------------------------------------
      double w;
      velnp.dot(*togglew_, &w);
      double p;
      velnp.dot(*togglep_, &p);

      //----------------------------------------------------------------------
      // add spatial mean values to statistical sample
      //----------------------------------------------------------------------
      int rnodnum = 0;
      (*zsumw_)(rnodnum, znodnum) += w;
      (*zsump_)(rnodnum, znodnum) += p;
    }
  }  // End of big loop along z-axis
  //----------------------------------------------------------------------
  //----------------------------------------------------------------------


  //----------------------------------------------------------------------
  //----------------------------------------------------------------------
  // Get mean velocity from evaluation planes
  //----------------------------------------------------------------------
  //----------------------------------------------------------------------
  // actual radial coordinate
  double actR = 0.0;

  for (unsigned actPosEval = 0; actPosEval < act_pos_evaluation_.size(); actPosEval++)
  {
    for (int rnodnum = 0; rnodnum < numrstatlocations_; rnodnum++)
    {
      // Get position
      actR = rcoordinates_(rnodnum, actPosEval);

      // Less nodes at the beginning -> only calculate until the last node and not until the matrix
      // is finished (0 else)
      if ((rcoordinates_(rnodnum, actPosEval) == 0.0) and (rnodnum != 0))
      {
        (*rsumw_)(rnodnum, actPosEval) = -1.0;
        (*rsump_)(rnodnum, actPosEval) = -1.0;
        continue;
      }

      togglew_->put_scalar(0.0);
      togglep_->put_scalar(0.0);

      int countnodes = 0;

      // get mean value of 4 nodes on a circle on the axis
      for (int circ = 0; circ < 4; circ++)
      {
        // Calculate coordinates of point
        double actX = sin(circ / 2.0 * 3.141592) * actR;
        double actY = cos(circ / 2.0 * 3.141592) * actR;

        // Because tolerances should not be added set nearly zero values to zero
        if (actX < 1e-8 and actX > -1e-8) actX = 0.0;
        if (actY < 1e-8 and actY > -1e-8) actY = 0.0;

        // Put 1.0 in toggle vectors if node on proc is desired node
        for (int nn = 0; nn < discret_->num_my_row_nodes(); ++nn)
        {
          Core::Nodes::Node* node = discret_->l_row_node(nn);

          // If node is on desired position then get toggle vector for pressure and velocity at
          // actual node
          if ((node->x()[1] < (actY + 2e-9) and node->x()[1] > (actY - 2e-9)) and
              (node->x()[0] < (actX + 2e-9) and node->x()[0] > (actX - 2e-9)) and
              (node->x()[2] < (act_pos_evaluation_[actPosEval] + 2e-9) and
                  node->x()[2] > (act_pos_evaluation_[actPosEval] - 2e-9)))
          {
            std::vector<int> dof = discret_->dof(node);
            double one = 1.0;
            togglep_->replace_global_values(1, &one, &(dof[3]));
            togglew_->replace_global_values(1, &one, &(dof[2]));
            countnodes++;
          }
        }
      }

      // Sum nodes on all procs (should be 4 nodes at a specific radius per evaluation plane except
      // of r=0 where it is 1)
      int countnodesonallprocs = 4;
      Core::Communication::sum_all(&countnodes, &countnodesonallprocs, 1, discret_->get_comm());

      // At r=0 the loop above wrote 4 times a 1.0 at the same position of the toggle vector so this
      // is a special case
      if (rcoordinates_(rnodnum, actPosEval) > -1e-9 and rcoordinates_(rnodnum, actPosEval) < 1e-9)
        countnodesonallprocs = 1;

      if (countnodesonallprocs)
      {
        // Get sum of values of 4 nodes at specific radius and evaluation plane
        double w;
        velnp.dot(*togglew_, &w);
        double p;
        velnp.dot(*togglep_, &p);

        // Calculate mean value
        double wsm = w / countnodesonallprocs;
        double psm = p / countnodesonallprocs;

        // Add mean value to matrix
        (*rsumw_)(rnodnum, actPosEval) += wsm;
        (*rsump_)(rnodnum, actPosEval) += psm;
      }
    }  // End of big loop along radial coordinate
  }  // End of loop through all evaluation planes
  //----------------------------------------------------------------------
  //----------------------------------------------------------------------

  return;
}  // TurbulenceStatisticsBfda::DoTimeSample


/*----------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------*/
void FLD::TurbulenceStatisticsBfda::dump_statistics(int step)
{
  //----------------------------------------------------------------------------------------------
  // file *.flow_statistics_along_z
  //----------------------------------------------------------------------------------------------
  std::shared_ptr<std::ofstream> log;
  std::shared_ptr<std::ofstream> log_2;
  if (Core::Communication::my_mpi_rank(discret_->get_comm()) == 0)
  {
    std::string s(statistics_outfilename_);
    s.append(".flow_statistics_along_z");  // s.append(".flow_statistics");
    log = std::make_shared<std::ofstream>(s.c_str(), std::ios::out);

    // Output of mean velocity and pressure along z-axis
    (*log) << "Output file of FDA blood flow benchmark - evaluation of z-axis                \n";
    (*log) << "------------------------------------------------------------------------------\n";
    (*log) << "                                                                              \n";
    (*log) << "Number of samples: " << numsamp_ << "                                         \n";
    (*log) << "\n";
    (*log) << "Positions_of_nodes_on_z-axis Mean_values_of_velocity_w_in_z-direction "
              "Mean_values_of_pressure_p_in_z-direction \n";

    for (unsigned i = 0; i < zcoordinates_->size(); ++i)
    {
      (*log) << " " << std::setw(11) << std::setprecision(4) << (*zcoordinates_)[i];
      (*log) << "   " << std::setw(11) << std::setprecision(4) << (*zsumw_)(0, i);
      (*log) << "   " << std::setw(11) << std::setprecision(4) << (*zsump_)(0, i);
      (*log) << "\n";
    }


    //----------------------------------------------------------------------------------------------
    // file *.flow_statistics_evaluation_planes
    //----------------------------------------------------------------------------------------------
    std::string s2(statistics_outfilename_);
    s2.append(".flow_statistics_evaluation_planes");
    log_2 = std::make_shared<std::ofstream>(s2.c_str(), std::ios::out);

    // Output of mean velocity and pressure of evaluation planes
    (*log_2) << "Output file of FDA blood flow benchmark - evaluation of evaluation planes     \n";
    (*log_2) << "------------------------------------------------------------------------------\n";
    (*log_2) << "                                                                              \n";
    (*log_2) << "Number_of_samples: " << numsamp_ << "                                         \n";
    (*log_2) << "\n\n";
    (*log_2) << "Radial_coordinate ";
    for (unsigned actEvalPlane = 0; actEvalPlane < pos_evaluation_.size(); actEvalPlane++)
      (*log_2) << "Evaluation_plane z-coordinate_of_evaluation_plane Radial_coordinate  "
                  "Mean_values_of_velocity_w_in_radial_direction "
                  "Mean_values_of_pressure_p_in_radial_direction ";

    (*log_2) << "\n";

    for (int i = 0; i < rcoordinates_.numRows(); ++i)
    {
      for (int j = 0; j < rcoordinates_.numCols(); ++j)
      {
        (*log_2) << " " << std::setw(11) << std::setprecision(4) << j + 1;
        (*log_2) << " " << std::setw(11) << std::setprecision(4) << act_pos_evaluation_[j];
        (*log_2) << " " << std::setw(11) << std::setprecision(4) << rcoordinates_[j][i];
        (*log_2) << "   " << std::setw(11) << std::setprecision(4) << (*rsumw_)(i, j);
        (*log_2) << "   " << std::setw(11) << std::setprecision(4) << (*rsump_)(i, j);
      }
      (*log_2) << "\n";
    }
    log->flush();
    log_2->flush();
  }
  return;
}  // TurbulenceStatisticsBfda::DumpStatistics

FOUR_C_NAMESPACE_CLOSE
