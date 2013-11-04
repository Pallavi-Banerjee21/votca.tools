/* 
 * Copyright 2009-2011 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <votca/tools/linalg.h>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <votca/tools/votca_config.h>

#ifndef NOGSL
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_eigen.h>
#endif


namespace votca { namespace tools {

using namespace std;


void linalg_invert( ub::matrix<double> &A, ub::matrix<double> &V){
    
#ifdef NOGSL
    throw std::runtime_error("linalg_invert is not compiled-in due to disabling of GSL - recompile Votca Tools with GSL support");
#else
        // matrix inversion using gsl
        // problem: after inversion, original matrix is overwritten!
        gsl_error_handler_t *handler = gsl_set_error_handler_off();
	const size_t N = A.size1();
	// signum s (for LU decomposition)
	int s;

        V.resize(N, N, false);
        
	// Define all the used matrices
        gsl_matrix_view A_view = gsl_matrix_view_array(&A(0,0), N, N);
        gsl_matrix_view V_view = gsl_matrix_view_array(&V(0,0), N, N);
	gsl_permutation * perm = gsl_permutation_alloc (N);
        
	// Make LU decomposition of matrix A_view
	gsl_linalg_LU_decomp (&A_view.matrix, perm, &s);

	// Invert the matrix A_view
	int status = gsl_linalg_LU_invert (&A_view.matrix, perm, &V_view.matrix);

        gsl_set_error_handler(handler);
        
	// return (status != 0);
    
#endif   
}


void linalg_transpose( ub::matrix<double> &A, ub::matrix<double> &V){
    
#ifdef NOGSL
    throw std::runtime_error("linalg_invert is not compiled-in due to disabling of GSL - recompile Votca Tools with GSL support");
#else
        // matrix transposition using gsl
        gsl_error_handler_t *handler = gsl_set_error_handler_off();
	const size_t N = A.size1();
	const size_t M = A.size2();
        V.resize(M, N, false);
	// Define all the used matrices
        gsl_matrix_view A_view = gsl_matrix_view_array(&A(0,0), N, M);
        gsl_matrix_view V_view = gsl_matrix_view_array(&V(0,0), M, N);

        int status = gsl_matrix_transpose_memcpy (&V_view.matrix, &A_view.matrix);

        gsl_set_error_handler(handler);
        
	// return (status != 0);
    
#endif   
}

void linalg_transpose( ub::matrix<double> &A){
    
#ifdef NOGSL
    throw std::runtime_error("linalg_transpose is not compiled-in due to disabling of GSL - recompile Votca Tools with GSL support");
#else
        // matrix transposition using gsl
        // after inversion, original matrix is overwritten!
        gsl_error_handler_t *handler = gsl_set_error_handler_off();
	const size_t N = A.size1();
	// Define all the used matrices
        gsl_matrix_view A_view = gsl_matrix_view_array(&A(0,0), N, N);

        // this only works on square matrices

        int status = gsl_matrix_transpose(&A_view.matrix);

        gsl_set_error_handler(handler);
        
	// return (status != 0);
    
#endif   
}



void linalg_cholesky_decompose( ub::matrix<double> &A){
    
#ifdef NOGSL
    throw std::runtime_error("linalg_cholesky_decompose is not compiled-in due to disabling of GSL - recompile Votca Tools with GSL support");
#else
        // Cholesky decomposition using GSL
        const size_t N = A.size1();
        
        gsl_matrix_view A_view = gsl_matrix_view_array(&A(0,0), N, N);
        
        // get the Cholesky matrices
        int status = gsl_linalg_cholesky_decomp ( &A_view.matrix );
        
#endif
}

void linalg_cholesky_solve(ub::vector<double> &x, ub::matrix<double> &A, ub::vector<double> &b){

#ifdef NOGSL
    throw std::runtime_error("linalg_cholesky_solve is not compiled-in due to disabling of GSL - recompile Votca Tools with GSL support");
#else
    /* calling program should catch the error error code GSL_EDOM
     * thrown by gsl_linalg_cholesky_decomp and take
     * necessary steps
     */
    
    gsl_matrix_view m
        = gsl_matrix_view_array (&A(0,0), A.size1(), A.size2());

    gsl_vector_view gb
        = gsl_vector_view_array (&b(0), b.size());

    gsl_vector *gsl_x = gsl_vector_alloc (x.size());

    gsl_set_error_handler_off();
    int status = gsl_linalg_cholesky_decomp(&m.matrix);

    if( status == GSL_EDOM)
        throw std::runtime_error("Matrix not symmetric positive definite");

    
    gsl_linalg_cholesky_solve(&m.matrix, &gb.vector, gsl_x);

    for (size_t i =0 ; i < x.size(); i++)
        x(i) = gsl_vector_get(gsl_x, i);

    gsl_vector_free (gsl_x);
#endif
}

void linalg_qrsolve(ub::vector<double> &x, ub::matrix<double> &A, ub::vector<double> &b, ub::vector<double> *residual)
{
#ifdef NOGSL
    throw std::runtime_error("linalg_qrsolve is not compiled-in due to disabling of GSL - recompile Votca Tools with GSL support");
#else
    // check matrix for zero column
    int nonzero_found = 0;
    for(size_t j=0; j<A.size2(); j++) {
        nonzero_found = 0;
        for(size_t i=0; i<A.size1(); i++) {
            if(fabs(A(i,j))>0) {
                nonzero_found = 1;
            }
        }
        if(nonzero_found==0) {
            throw "qrsolve_zero_column_in_matrix";
        }
    }

    gsl_matrix_view m
        = gsl_matrix_view_array (&A(0,0), A.size1(), A.size2());

    gsl_vector_view gb
        = gsl_vector_view_array (&b(0), b.size());

    gsl_vector *gsl_x = gsl_vector_alloc (x.size());
    gsl_vector *tau = gsl_vector_alloc (x.size());
    gsl_vector *gsl_residual = gsl_vector_alloc (b.size());

    gsl_linalg_QR_decomp (&m.matrix, tau);

    gsl_linalg_QR_lssolve (&m.matrix, tau, &gb.vector, gsl_x, gsl_residual);

    for (size_t i =0 ; i < x.size(); i++)
        x(i) = gsl_vector_get(gsl_x, i);

    if(residual)
        for (size_t i =0 ; i < residual->size(); i++)
            (*residual)(i) = gsl_vector_get(gsl_residual, i);

    gsl_vector_free (gsl_x);
    gsl_vector_free (tau);
    gsl_vector_free (gsl_residual);
#endif
}

void linalg_constrained_qrsolve(ub::vector<double> &x, ub::matrix<double> &A, ub::vector<double> &b, ub::matrix<double> &constr)
{
#ifdef NOGSL
    throw std::runtime_error("linalg_constrained_qrsolve is not compiled-in due to disabling of GSL - recompile Votca Tools with GSL support");
#else
    // check matrix for zero column
    int nonzero_found = 0;
    for(size_t j=0; j<A.size2(); j++) {
        nonzero_found = 0;
        for(size_t i=0; i<A.size1(); i++) {
            if(fabs(A(i,j))>0) {
                nonzero_found = 1;
            }
        }
        if(nonzero_found==0) {
            throw std::runtime_error("constrained_qrsolve_zero_column_in_matrix");
        }
    }

    // Transpose constr:
    constr = trans(constr);

    const int N = b.size();
    const int ngrid = x.size()/2;

    // temporary variables
    ub::matrix<double> Q(2*ngrid, 2*ngrid);       // Q matrix: QR decomposition of trans(B)
    ub::matrix<double> Q_k(2*ngrid, 2*ngrid);
    ub::identity_matrix<double> I (2*ngrid);
    ub::vector<double> v(2*ngrid);

    Q = ub::zero_matrix<double>(2*ngrid, 2*ngrid);
    Q_k = ub::zero_matrix<double>(2*ngrid, 2*ngrid);
    v = ub::zero_vector<double>(2*ngrid);

    double *tmp = & constr(0,0);
    gsl_matrix_view gsl_constr
      = gsl_matrix_view_array (tmp, constr.size1(), constr.size2());

    tmp = &b(0);
    gsl_vector_view gsl_b
         = gsl_vector_view_array (tmp, b.size());


    gsl_vector *tau_qr = gsl_vector_alloc (ngrid);

    gsl_linalg_QR_decomp (&gsl_constr.matrix, tau_qr);

    Q = I;

    for (int k = ngrid; k > 0 ; k--) {

        for (int icout = 0; icout < k - 1; icout++) {
             v(icout) = 0;
        }
        v(k - 1) = 1.0;

        for (int icout = k; icout < 2*ngrid; icout++) {
             v(icout) = gsl_matrix_get(&gsl_constr.matrix, icout, k - 1 );
        }

        Q_k = I - gsl_vector_get(tau_qr, k - 1 ) * outer_prod ( v, v );
        Q = prec_prod(Q, Q_k);

    }

    Q = trans(Q);
    gsl_vector_free (tau_qr);

    // Calculate A * Q and store the result in A
    A = prec_prod(A, Q);


    // A = [A1 A2], so A2 is just a block of A
    ub::matrix<double> A2 = ub::matrix_range<ub::matrix<double> >(A,
            ub::range (0, N), ub::range (ngrid, 2*ngrid)
         );

    tmp = &A2(0,0);
    gsl_matrix_view gsl_A2
         = gsl_matrix_view_array (tmp, A2.size1(), A2.size2());
   
        
    gsl_vector *z = gsl_vector_alloc (ngrid);
    gsl_vector *tau_solve = gsl_vector_alloc (ngrid);  // already done!
    gsl_vector *residual = gsl_vector_alloc (N);

    gsl_linalg_QR_decomp (&gsl_A2.matrix, tau_solve);
    gsl_linalg_QR_lssolve (&gsl_A2.matrix, tau_solve, &gsl_b.vector, z, residual);

    // Next two cycles assemble vector from y (which is zero-vector) and z
    // (which we just got by gsl_linalg_QR_lssolve)

    for (int i = 0; i < ngrid; i++ ) {
           x[i] = 0.0;
    }

    for (int i = ngrid; i < 2 * ngrid; i++ ) {
           x[i] = gsl_vector_get(z, i - ngrid);
    }

    // To get the final answer this vector should be multiplied by matrix Q
    // TODO: here i changed the sign, check again! (victor)
    x = -prec_prod( Q, x );

    gsl_vector_free (z);
    gsl_vector_free (tau_solve);
    gsl_vector_free (residual);
#endif
}


    
/**
*
* ublas binding for gsl_eigen_symmv
* note that the eigenvalues/eigenvectors are UNSORTED 
* 
*/
bool linalg_eigenvalues_symmetric( ub::symmetric_matrix<double> &A, ub::vector<double> &E, ub::matrix<double> &V)
{
#ifdef NOGSL
    throw std::runtime_error("linalg_eigenvalues_symmetric is not compiled-in due to disabling of GSL - recompile Votca Tools with GSL support");
#else
    
	gsl_error_handler_t *handler = gsl_set_error_handler_off();
	const size_t N = A.size1();
        
        // gsl does not handle conversion of a symmetric_matrix 
        ub::matrix<double> _A( N,N );
        _A = A;
        
	E.resize(N, false);
	V.resize(N, N, false);
	gsl_matrix_view A_view = gsl_matrix_view_array(&_A(0,0), N, N);
	gsl_vector_view E_view = gsl_vector_view_array(&E(0), N);
	gsl_matrix_view V_view = gsl_matrix_view_array(&V(0,0), N, N);
	gsl_eigen_symmv_workspace *w = gsl_eigen_symmv_alloc(N);

	int status = gsl_eigen_symmv(&A_view.matrix, &E_view.vector, &V_view.matrix, w);
	//gsl_eigen_symmv_sort(&E_view.vector, &V_view.matrix, GSL_EIGEN_SORT_ABS_ASC);
	gsl_eigen_symmv_free(w);
	gsl_set_error_handler(handler);
        
	return (status != 0);
#endif
};


/**
*
* ublas binding for gsl_eigen_symmv
* note that the eigenvalues/eigenvectors are UNSORTED 
* input matrix type general matrix! 
* 
*/
bool linalg_eigenvalues( ub::matrix<double> &A, ub::vector<double> &E, ub::matrix<double> &V)
{
#ifdef NOGSL
    throw std::runtime_error("linalg_eigenvalues is not compiled-in due to disabling of GSL - recompile Votca Tools with GSL support");
#else
    
	gsl_error_handler_t *handler = gsl_set_error_handler_off();
	const size_t N = A.size1();
        
        // gsl does not handle conversion of a symmetric_matrix 
        ub::matrix<double> _A( N,N );
        _A = A;
        
	E.resize(N, false);
	V.resize(N, N, false);
	gsl_matrix_view A_view = gsl_matrix_view_array(&_A(0,0), N, N);
	gsl_vector_view E_view = gsl_vector_view_array(&E(0), N);
	gsl_matrix_view V_view = gsl_matrix_view_array(&V(0,0), N, N);
	gsl_eigen_symmv_workspace *w = gsl_eigen_symmv_alloc(N);

	int status = gsl_eigen_symmv(&A_view.matrix, &E_view.vector, &V_view.matrix, w);
	gsl_eigen_symmv_sort(&E_view.vector, &V_view.matrix, GSL_EIGEN_SORT_ABS_ASC);
	gsl_eigen_symmv_free(w);
	gsl_set_error_handler(handler);
        
	return (status != 0);
#endif
};




/**
 * ublas binding to GSL  Singular Value Decomposition
 * 
 * A = U S V^T
 * 
 * @param A MxN matrix do decompose. Becomes an MxN orthogonal matrix U
 * @param V NxN orthogonal square matrix
 * @param E NxN diagonal matrix of singular values
 * @return succeeded or not 
 */
bool linalg_singular_value_decomposition( ub::matrix<double> &A, ub::matrix<double> &V, ub::vector<double> &S )
{
	/*
        gsl_error_handler_t *handler = gsl_set_error_handler_off();
	const size_t N = A.size1();
        
        // gsl does not handle conversion of a symmetric_matrix 
        ub::matrix<double> _A( N,N );
        _A = A;
        
	E.resize(N, false);
	V.resize(N, N, false);
	gsl_matrix_view A_view = gsl_matrix_view_array(&_A(0,0), N, N);
	gsl_vector_view E_view = gsl_vector_view_array(&E(0), N);
	gsl_matrix_view V_view = gsl_matrix_view_array(&V(0,0), N, N);
	gsl_eigen_symmv_workspace *w = gsl_eigen_symmv_alloc(N);

	int status = gsl_eigen_symmv(&A_view.matrix, &E_view.vector, &V_view.matrix, w);
	//gsl_eigen_symmv_sort(&E_view.vector, &V_view.matrix, GSL_EIGEN_SORT_ABS_ASC);
	gsl_eigen_symmv_free(w);
	gsl_set_error_handler(handler);
        
	return (status != 0);
         */
    throw std::runtime_error("Singular Value Decomposition is not implemented");
    return false;
};


}}
