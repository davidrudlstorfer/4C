/*----------------------------------------------------------------------*/
/*!
\file beam3r_lin2_linkage.cpp

\brief Wrapper for a linear Reissner beam element used as mechanical link between two other beam elements

\level 3

\maintainer Maximilian Grill
*/
/*----------------------------------------------------------------------*/

#include "beam3r_lin2_linkage.H"
#include "beam_to_beam_linkage.H"

#include "../drt_beam3/beam3r.H"

#include "../drt_fem_general/largerotations.H"

#include "../linalg/linalg_serialdensematrix.H"
#include "../linalg/linalg_serialdensevector.H"

#include "../drt_lib/drt_dserror.H"
#include "../drt_lib/drt_utils_factory.H"

#include <Teuchos_RCP.hpp>


BEAMINTERACTION::Beam3rLin2LinkageType BEAMINTERACTION::Beam3rLin2LinkageType::instance_;


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
DRT::ParObject* BEAMINTERACTION::Beam3rLin2LinkageType::Create( const std::vector<char> & data )
{
  BEAMINTERACTION::Beam3rLin2Linkage * my_beam3rlin2 = new BEAMINTERACTION::Beam3rLin2Linkage();
  my_beam3rlin2->Unpack(data);
  return my_beam3rlin2;
}


/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
BEAMINTERACTION::Beam3rLin2Linkage::Beam3rLin2Linkage() :
    BeamToBeamLinkage(),
    linkele_(Teuchos::null)
{
  // empty constructor
  return;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void BEAMINTERACTION::Beam3rLin2Linkage::Setup( const int matnum )
{
  CheckInit();

  // call setup of base class first
  BeamToBeamLinkage::Setup( matnum );

  /* the idea is to use a beam element as auxiliary object that provides us with a
   * response force (and moment) depending on the position and orientation of the
   * two material cross-sections (binding spots) it is connected to;
   *
   * note: the element instance created in this way can only be used in a limited way
   *       because it is not embedded in a discretization. For example,
   *       Nodes() and other methods are not functional because the
   *       pointers to nodes are not set. Same for reference position of nodes via X() ...
   *
   *       We really only use it as a calculation routine for a sophisticated
   *       (displacement-reaction force) relation here! */
  linkele_ = Teuchos::rcp(new DRT::ELEMENTS::Beam3r(-1,0));

  // set material
  linkele_->SetMaterial(matnum);

  // Todo @grill: safety check for proper material type (done on element anyway, but do it here as well)?!

  linkele_->SetCenterlineHermite(false);

  // set dummy node Ids, in order to make NumNodes() method of element return the correct number of nodes
  int nodeids[2];
  for (unsigned int i=0; i<2; ++i) nodeids[i]=-1;
  linkele_->SetNodeIds(2,&nodeids[0]);


  // the triads at the two connection sites are chosen identical initially, so we only use the first one
  LINALG::Matrix<3,1> linkelerotvec(true);
  LARGEROTATIONS::quaterniontoangle(GetBindSpotQuaternion1(),linkelerotvec);

  std::vector<double> refpos(6,0.0);
  std::vector<double> refrotvec(6,0.0);

  for (unsigned int i=0; i<3; ++i)
  {
    refpos[i] = GetBindSpotPos1()(i);
    refpos[3+i] = GetBindSpotPos2()(i);

    refrotvec[i] = linkelerotvec(i);
    refrotvec[3+i] = linkelerotvec(i);
  }

  linkele_->SetUpReferenceGeometry<2,2,1>(refpos,refrotvec);

//  std::cout << "\nSetup():";
//  this->Print(std::cout);

  issetup_ = true;
}

/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
void BEAMINTERACTION::Beam3rLin2Linkage::Pack(DRT::PackBuffer& data) const
{
  CheckInitSetup();

  DRT::PackBuffer::SizeMarker sm( data );
  sm.Insert();

  // pack type of this instance of ParObject
  int type = UniqueParObjectId();
  AddtoPack(data,type);
  // add base class
  BeamToBeamLinkage::Pack(data);

  // pack linker element
  if (linkele_!=Teuchos::null)
    linkele_->Pack(data);

  return;
}

/*----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
void BEAMINTERACTION::Beam3rLin2Linkage::Unpack(const std::vector<char>& data)
{
  std::vector<char>::size_type position = 0;
  // extract type
  int type = 0;
  ExtractfromPack(position,data,type);
  if (type != UniqueParObjectId()) dserror("wrong instance type data");
  // extract base class
  std::vector<char> basedata(0);
  ExtractfromPack(position,data,basedata);
  BeamToBeamLinkage::Unpack(basedata);

  // Unpack data of sub material (these lines are copied from drt_element.cpp)
  std::vector<char> dataele;
  ExtractfromPack( position, data, dataele );
  if ( dataele.size() > 0 )
  {
    DRT::ParObject* object = DRT::UTILS::Factory(dataele);  // Unpack is done here
    DRT::ELEMENTS::Beam3r* linkele = dynamic_cast<DRT::ELEMENTS::Beam3r*>(object);
    if (linkele==NULL)
      dserror("failed to unpack Beam3r object within Beam3rLin2Linkage");
    linkele_ = Teuchos::rcp(linkele);
  }
  else linkele_ = Teuchos::null;

  return;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
bool BEAMINTERACTION::Beam3rLin2Linkage::EvaluateForce(
    LINALG::SerialDenseVector& forcevec1,
    LINALG::SerialDenseVector& forcevec2)
{
  CheckInitSetup();

  LINALG::TMatrix<double,6,1> disp_totlag_centerline;
  std::vector<LINALG::TMatrix<double,4,1> > Qnode;

  FillStateVariablesForElementEvaluation(
      disp_totlag_centerline,
      Qnode);

  LINALG::SerialDenseVector force(12,true);

  linkele_->CalcInternalAndInertiaForcesAndStiff<2,2,1>(
      disp_totlag_centerline,
      Qnode,
      NULL,
      NULL,
      &force,
      NULL);

  // Todo maybe we can avoid this copy by setting up 'force' as a view on the
  //      two separate force vectors ?
  std::copy( &force(0), &force(0)+6, &forcevec1(0) );
  std::copy( &force(0)+6, &force(0)+12, &forcevec2(0) );

  return true;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
bool BEAMINTERACTION::Beam3rLin2Linkage::EvaluateStiff(
    LINALG::SerialDenseMatrix& stiffmat11,
    LINALG::SerialDenseMatrix& stiffmat12,
    LINALG::SerialDenseMatrix& stiffmat21,
    LINALG::SerialDenseMatrix& stiffmat22)
{
  CheckInitSetup();

  LINALG::TMatrix<double,6,1> disp_totlag_centerline;
  std::vector<LINALG::TMatrix<double,4,1> > Qnode;

  FillStateVariablesForElementEvaluation(
      disp_totlag_centerline,
      Qnode);

  LINALG::SerialDenseMatrix stiffmat(12,12,true);

  linkele_->CalcInternalAndInertiaForcesAndStiff<2,2,1>(
      disp_totlag_centerline,
      Qnode,
      &stiffmat,
      NULL,
      NULL,
      NULL);

  // Todo can we use std::copy here or even set up 'stiffmat' as a view on the
  //      four individual sub-matrices ?
  for (unsigned int i=0; i<6; ++i)
    for (unsigned int j=0; j<6; ++j)
    {
      stiffmat11(i,j) = stiffmat(i,j);
      stiffmat12(i,j) = stiffmat(i,6+j);
      stiffmat21(i,j) = stiffmat(6+i,j);
      stiffmat22(i,j) = stiffmat(6+i,6+j);
    }

  return true;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
bool BEAMINTERACTION::Beam3rLin2Linkage::EvaluateForceStiff(
    LINALG::SerialDenseVector& forcevec1,
    LINALG::SerialDenseVector& forcevec2,
    LINALG::SerialDenseMatrix& stiffmat11,
    LINALG::SerialDenseMatrix& stiffmat12,
    LINALG::SerialDenseMatrix& stiffmat21,
    LINALG::SerialDenseMatrix& stiffmat22)
{
  CheckInitSetup();

  LINALG::TMatrix<double,6,1> disp_totlag_centerline;
  std::vector<LINALG::TMatrix<double,4,1> > Qnode;

  FillStateVariablesForElementEvaluation(
      disp_totlag_centerline,
      Qnode);

  LINALG::SerialDenseVector force(12,true);
  LINALG::SerialDenseMatrix stiffmat(12,12,true);

  linkele_->CalcInternalAndInertiaForcesAndStiff<2,2,1>(
      disp_totlag_centerline,
      Qnode,
      &stiffmat,
      NULL,
      &force,
      NULL);

  std::copy( &force(0), &force(0)+6, &forcevec1(0) );
  std::copy( &force(0)+6, &force(0)+12, &forcevec2(0) );

  for (unsigned int i=0; i<6; ++i)
  {
    for (unsigned int j=0; j<6; ++j)
    {
      stiffmat11(i,j) = stiffmat(i,j);
      stiffmat12(i,j) = stiffmat(i,6+j);
      stiffmat21(i,j) = stiffmat(6+i,j);
      stiffmat22(i,j) = stiffmat(6+i,6+j);
    }
  }

  return true;
}

/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/
void BEAMINTERACTION::Beam3rLin2Linkage::FillStateVariablesForElementEvaluation(
    LINALG::TMatrix<double,6,1>&               disp_totlag_centerline,
    std::vector<LINALG::TMatrix<double,4,1> >& Qnode
    ) const
{
  for (unsigned int i=0; i<3; ++i)
  {
    disp_totlag_centerline(i) = GetBindSpotPos1()(i);
    disp_totlag_centerline(3+i) = GetBindSpotPos2()(i);
  }

  Qnode.push_back(GetBindSpotQuaternion1());
  Qnode.push_back(GetBindSpotQuaternion2());
}