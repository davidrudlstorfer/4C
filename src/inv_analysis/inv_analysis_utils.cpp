/*----------------------------------------------------------------------*/
/*! \file
\brief Utilities for the inverse analysis

\level 3

*/
/*----------------------------------------------------------------------*/

#include "inv_analysis_utils.H"

#include "lib_dserror.H"
#include "Epetra_SerialDenseVector.h"
#include "linalg_utils_sparse_algebra_manipulation.H"
#include "inv_analysis_DcsMatrix.H"
#include "inv_analysis_chol_factory.H"
#include "inpar_statinvanalysis.H"

#include "Teuchos_ParameterList.hpp"
#include "Teuchos_StandardParameterEntryValidators.hpp"

#include <fstream>


/*----------------------------------------------------------------------*/
/* Compute norm of two vectors stored in                    keh 03/14   */
/* multivector format for storage reasons only                          */
/*----------------------------------------------------------------------*/
void INVANA::MVNorm(
    const Epetra_MultiVector& avector, const Epetra_Map& uniquemap, int anorm, double* result)
{
  Epetra_SerialDenseVector vnorm(avector.NumVectors());

  Teuchos::RCP<Epetra_MultiVector> unique =
      Teuchos::rcp(new Epetra_MultiVector(uniquemap, avector.NumVectors(), true));
  LINALG::Export(avector, *unique);

  if (anorm == 2)
    unique->Norm2(vnorm.Values());
  else if (anorm == 0)
    unique->NormInf(vnorm.Values());
  else if (anorm == 1)
    unique->Norm1(vnorm.Values());
  else
    dserror("provide norm to be computed: 0 (inf-Norm), 1 (1-Norm) or 2 (2-Norm)");

  *result = vnorm.Norm2();
}


/*----------------------------------------------------------------------*/
/* Compute dot product of two vectors stored in             keh 03/14   */
/* multivector format for storage reasons only                          */
/*----------------------------------------------------------------------*/
void INVANA::MVDotProduct(const Epetra_MultiVector& avector, const Epetra_MultiVector& bvector,
    const Epetra_Map& uniquemap, double* result)
{
  dsassert(avector.NumVectors() == bvector.NumVectors(), "give proper multivectors!");

  Epetra_SerialDenseVector anorm(avector.NumVectors());

  Teuchos::RCP<Epetra_MultiVector> uniquea =
      Teuchos::rcp(new Epetra_MultiVector(uniquemap, avector.NumVectors(), true));
  LINALG::Export(avector, *uniquea);
  Teuchos::RCP<Epetra_MultiVector> uniqueb =
      Teuchos::rcp(new Epetra_MultiVector(uniquemap, bvector.NumVectors(), true));
  LINALG::Export(bvector, *uniqueb);

  // do dot product with unique vectors now
  uniquea->Dot(*uniqueb, anorm.Values());

  *result = 0.0;
  for (int j = 0; j < anorm.Length(); j++) *result += anorm[j];
}


/*----------------------------------------------------------------------*/
Teuchos::RCP<INVANA::CholFactorBase> INVANA::CreateICT(
    Teuchos::RCP<Epetra_CrsMatrix> covariance, const Teuchos::ParameterList& params)
{
  INVANA::CholFactory cholfactory;

  // when different factorizations are derived from
  // INVANA::CholFactorBase. Put the necessary stuff into this list
  // the only current implementation doesn't need any input
  Teuchos::ParameterList List(params);

  Teuchos::RCP<CholFactorBase> prec = cholfactory.Create(covariance, List);

  return prec;
}