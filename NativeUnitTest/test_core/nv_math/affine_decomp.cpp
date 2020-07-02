/*********************************************************************NVMH1****
File:
affine_decomp.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments: 

This code has been ripped off of :
Graphics Gem IV, Paul Heckbert - Polar Matrix Decomposition from Ken Shoemake p207.

    - Sébastien Domin?

******************************************************************************/

#ifndef _nv_algebra_h_
#include "nv_math.h"
#endif

#ifndef _WIN32
#define _isnan isnan
#define _finite finite
#endif

// Return index of column of M containing maximum abs entry, or -1 if M=0
int find_max_col(mat3 & M)
{
    nv_scalar   abs, max = nv_zero;
    int         col = -1;

    for (int i = 0; i < 3; ++i) 
    {
        for (int j = 0; j < 3; ++j) 
        {
            abs = M(i,j); 
            if (abs < nv_zero) 
                abs = -abs;
            if (abs > max) 
            {
                max = abs; 
                col = j;
            }
        }
    }
    return col;
}

// Setup u for Household reflection to zero all v components but first
void make_reflector(const vec3 &v, vec3 &u)
{
    nv_scalar s = sqrtf(dot(v, v));
    u = v; 
    u.z += ((v.z < nv_zero) ? -s : s);
    s = sqrtf(nv_two / dot(u, u));
    u *= s; 
}

// Apply Householder reflection represented by u to column vectors of M
void reflect_cols(mat3 &M, vec3 &u)
{
    for (int i=0; i < 3; ++i) 
    {
        nv_scalar s = dot(u , M.col(i));
        for (int j=0; j < 3; ++j) 
            M(j,i) -= u[j]*s;
    }
}
// Apply Householder reflection represented by u to row vectors of M
void reflect_rows(mat3 & M, vec3 &u)
{
    for (int i=0; i<3; ++i) 
    {
        nv_scalar s = dot(u, M[i]);
        for (int j=0; j < 3; ++j) 
            M(i,j) -= u[j]*s;
    }
}

// Find orthogonal factor Q of rank 1 (or less) M
void do_rank1(mat3 & M, mat3& Q)
{
    vec3        v1, v2;
    nv_scalar   s;
    int         col;

    Q = mat3_id;
    /* If rank(M) is 1, we should find a non-zero column in M */
    col = find_max_col(M);
    if ( col < 0 ) 
        return; /* Rank is 0 */
    v1 = M.col(col); 

    make_reflector(v1, v1); 
    reflect_cols(M, v1);

    v2[0] = M[2][0]; v2[1] = M[2][1]; v2[2] = M[2][2];
    make_reflector(v2, v2); reflect_rows(M, v2);
    s = M[2][2];
    if (s < nv_zero) 
        Q(2,2) = -nv_one;
    reflect_cols(Q, v1); reflect_rows(Q, v2);
}

// Find orthogonal factor Q of rank 2 (or less) M using adjoint transpose
void do_rank2(mat3& M, mat3 &MadjT, mat3& Q)
{
    vec3        v1, v2;
    nv_scalar   w, x, y, z, c, s, d;
    int         col;
    // If rank(M) is 2, we should find a non-zero column in MadjT
    col = find_max_col(MadjT);
    if (col<0) 
    {
        do_rank1(M, Q); 
        return;
    } // Rank<2
    v1[0] = MadjT[0][col]; 
    v1[1] = MadjT[1][col]; 
    v1[2] = MadjT[2][col];
    make_reflector(v1, v1); 
    reflect_cols(M, v1);
    cross(v2, M[0], M[1]);
    make_reflector(v2, v2); 
    reflect_rows(M, v2);
    w = M[0][0]; 
    x = M[0][1]; 
    y = M[1][0]; 
    z = M[1][1];
    if (w*z>x*y) 
    {
        c = z+w; s = y-x; d = sqrtf(c*c+s*s); c = c/d; s = s/d;
        Q(0,0) = Q(1,1) = c; Q(0,1) = -(Q(1,0) = s);
    }
    else 
    {
        c = z-w; s = y+x; d = sqrtf(c*c+s*s); c = c/d; s = s/d;
        Q(0,0) = -(Q(1,1) = c); Q(0,1) = Q(1,0) = s;
    }
    Q(0,2) = Q(2,0) = Q(1,2) = Q(2,1) = nv_zero; Q(2,2) = nv_one;
    reflect_cols(Q, v1); 
    reflect_rows(Q, v2);
}

//******* Polar Decomposition *******

/* Polar Decomposition of 3x3 matrix in 4x4,
 * M = QS.  See Nicholas Higham and Robert S. Schreiber,
 * Fast Polar Decomposition of An Arbitrary Matrix,
 * Technical Report 88-942, October 1988,
 * Department of Computer Science, Cornell University.
 */
float polar_decomp(const mat3 & M, mat3 & Q, mat3 & S)
{
    mat3 tmp;
    mat3 Mk, MadjTk, Ek;

    nv_scalar d, M_one, M_inf, MadjT_one, MadjT_inf, E_one, gamma, g1, g2;

    transpose(Mk,M);
    M_one = Mk.norm_one();  
    M_inf = Mk.norm_inf();

    do {
        invert(MadjTk,Mk);
        transpose(MadjTk);
        MadjTk*=det(MadjTk);

        d = dot(Mk.col(0), MadjTk.col(0));

        if (d == nv_zero) 
        {
            do_rank2(Mk, MadjTk, Mk);
            break;
        }

        MadjT_one = MadjTk.norm_one(); 
        MadjT_inf = MadjTk.norm_inf();

        gamma = sqrtf(sqrtf((MadjT_one*MadjT_inf)/(M_one*M_inf))/fabs(d));

        g1 = gamma*nv_zero_5;
        g2 = nv_zero_5 / (gamma*d);

        Ek = Mk;
        tmp = Mk;
        tmp *= g1;
        MadjTk *= g2;
        add(Mk,tmp,MadjTk);
        Ek-=Mk;

        E_one = Ek.norm_one();
        M_one = Mk.norm_one();  
        M_inf = Mk.norm_inf();
    } 
    while (E_one>(M_one*nv_eps));

    transpose(Q,Mk); 
    mult(S, Mk, M);  
    
    nv_scalar x;
    for (int i = 0; i < 3; ++i) 
    {
        for (int j = i; j < 3; ++j)
        {
            x = nv_zero_5 * (S[i][j]+S[j][i]);
            S(i,j) = x;
            S(j,i) = x;
        }
    }
    return (d);
}

//******* Spectral Decomposition *******

/* Compute the spectral decomposition of symmetric positive semi-definite S.
 * Returns rotation in U and scale factors in result, so that if K is a diagonal
 * matrix of the scale factors, then S = U K (U transpose). Uses Jacobi method.
 * See Gene H. Golub and Charles F. Van Loan. Matrix Computations. Hopkins 1983.
 */

vec3 & spect_decomp(vec3 & kv, mat3 & S, mat3 & U)
{
#define X 0
#define Y 1
#define Z 2
#define W 3

    vec3 Diag,OffD; // OffD is off-diag (by omitted index)
    nv_scalar g,h,fabsh,fabsOffDi,t,theta,c,s,tau,ta,OffDq,a,b;
    static char nxt[] = {Y,Z,X};

    U = mat3_id;
    Diag[X] = S(X,X); Diag[Y] = S(Y,Y); Diag[Z] = S(Z,Z);
    OffD[X] = S(Y,Z); OffD[Y] = S(Z,X); OffD[Z] = S(X,Y);
    for (int sweep=20; sweep>0; --sweep) 
    {
        nv_scalar sm = (fabs(OffD[X])+fabs(OffD[Y])+fabs(OffD[Z]));
        if (sm == nv_zero) 
            break;
        for (int i=Z; i>=X; --i) 
        {
            int p = nxt[i]; 
            int q = nxt[p];
            fabsOffDi = fabs(OffD[i]);
            g = nv_scalar(100.0)*fabsOffDi;
            if (fabsOffDi > nv_zero) 
            {
                h = Diag[q] - Diag[p];
                fabsh = fabs(h);
                if (fabsh+g==fabsh) 
                {
                    t = OffD[i]/h;
                } 
                else 
                {
                    theta = nv_zero_5*h/OffD[i];
                    t = nv_one/(fabs(theta)+sqrt(theta*theta+nv_one));
                    if (theta < nv_zero) 
                        t = -t;
                }
                c = nv_one/sqrt(t*t+nv_one); 
                s = t*c;
                tau = s/(c+nv_one);
                ta = t*OffD[i]; 
                OffD[i] = nv_zero;
                Diag[p] -= ta; 
                Diag[q] += ta;
                OffDq = OffD[q];
                OffD[q] -= s*(OffD[p] + tau*OffD[q]);
                OffD[p] += s*(OffDq   - tau*OffD[p]);
                for (int j=Z; j>=X; --j) 
                {
                    a = U(j,p); b = U(j,q);
                    U(j,p) -= s*(b + tau*a);
                    U(j,q) += s*(a - tau*b);
                }
            }
        }
    }
    kv = Diag; 
    return kv;
}

/******* Spectral Axis Adjustment *******/

/* Given a unit quaternion, q, and a scale vector, k, find a unit quaternion, p,
 * which permutes the axes and turns freely in the plane of duplicate scale
 * factors, such that q p has the largest possible w component, i.e. the
 * smallest possible angle. Permutes k's components to go with q p instead of q.
 * See Ken Shoemake and Tom Duff. Matrix Animation and Polar Decomposition.
 * Proceedings of Graphics Interface 1992. Details on p. 262-263.
 */
quat & snuggle(quat & p, quat & q, vec3 & k)
{
#define sgn(n,v)    ((n)?-(v):(v))
#define swap(a,i,j) {a[3]=a[i]; a[i]=a[j]; a[j]=a[3];}
#define cycle(a,p)  if (p) {a[3]=a[0]; a[0]=a[1]; a[1]=a[2]; a[2]=a[3];}\
                    else   {a[3]=a[2]; a[2]=a[1]; a[1]=a[0]; a[0]=a[3];}
    vec4 ka;
    int i, turn = -1;
    ka = k;
    if (ka[X]==ka[Y]) 
    {
        if (ka[X]==ka[Z]) 
            turn = W; 
        else 
            turn = Z;
    }
    else {if (ka[X]==ka[Z]) turn = Y; else if (ka[Y]==ka[Z]) turn = X;}
    if (turn>=0) 
    {
        quat qtoz, qp;
        unsigned neg[3], win;
        double mag[3], t;
        static quat qxtoz = quat(0.0f,nv_sqrthalf,0.0f,nv_sqrthalf);
        static quat qytoz = quat(nv_sqrthalf,0.0f,0.0f,nv_sqrthalf);
        static quat qppmm = quat(0.5, 0.5,-0.5,-0.5);
        static quat qpppp = quat(0.5, 0.5, 0.5, 0.5);
        static quat qmpmm = quat(-0.5, 0.5,-0.5,-0.5);
        static quat qpppm = quat(0.5, 0.5, 0.5,-0.5);
        static quat q0001 = quat(0.0, 0.0, 0.0, 1.0);
        static quat q1000 = quat(1.0, 0.0, 0.0, 0.0);
        switch (turn) 
        {
            default: 
                return (conj(p,q));
            case X: 
                qtoz = qxtoz;
                q *= qtoz; 
                swap(ka,X,Z) 
                break;
            case Y: 
                qtoz = qytoz;
                q *= qtoz; 
                swap(ka,Y,Z) 
                break;
            case Z: 
                qtoz = q0001; 
                break;
        }
        conj(q);
        mag[0] = (double)q.z*q.z+(double)q.w*q.w - 0.5;
        mag[1] = (double)q.x*q.z-(double)q.y*q.w;
        mag[2] = (double)q.y*q.z+(double)q.x*q.w;
        for (i=0; i<3; i++) 
            if (neg[i] = (mag[i]<0.0)) 
                mag[i] = -mag[i];
        if (mag[0]>mag[1]) 
        {
            if (mag[0]>mag[2]) 
                win = 0; 
            else 
                win = 2;
        }
        else               
        {
            if (mag[1]>mag[2]) 
                win = 1; 
            else 
                win = 2;
        }
        switch (win) 
        {
            case 0: 
                if (neg[0]) 
                    p = q1000; 
                else 
                    p = q0001; 
                break;
            case 1: 
                if (neg[1]) 
                    p = qppmm; 
                else p = qpppp; 
                cycle(ka,0) 
                break;
            case 2: 
                if (neg[2]) 
                    p = qmpmm; 
                else 
                    p = qpppm; 
                cycle(ka,1) 
                break;
        }
        add_quats(qp, q, p);
        t = sqrt(mag[win]+0.5);
        p *= quat(nv_zero,nv_zero,-qp.z/t,qp.w/t);
        conj(p);
        p *= qtoz;
    }
    else 
    {
        float qa[4], pa[4];
        unsigned lo, hi, neg[4], par = 0;
        double all, big, two;
        qa[0] = q.x; qa[1] = q.y; qa[2] = q.z; qa[3] = q.w;
        for (i=0; i<4; i++) {
            pa[i] = 0.0;
            if (neg[i] = (qa[i]<0.0)) qa[i] = -qa[i];
            par ^= neg[i];
        }
        /* Find two largest components, indices in hi and lo */
        if (qa[0]>qa[1]) lo = 0; else lo = 1;
        if (qa[2]>qa[3]) hi = 2; else hi = 3;
        if (qa[lo]>qa[hi]) {
            if (qa[lo^1]>qa[hi]) {hi = lo; lo ^= 1;}
            else {hi ^= lo; lo ^= hi; hi ^= lo;}
        } else {if (qa[hi^1]>qa[lo]) lo = hi^1;}
        all = (qa[0]+qa[1]+qa[2]+qa[3])*0.5;
        two = (qa[hi]+qa[lo])*nv_sqrthalf;
        big = qa[hi];
        if (all>two) {
            if (all>big) {/*all*/
                {int i; for (i=0; i<4; i++) pa[i] = sgn(neg[i], 0.5);}
                cycle(ka,par)
            } else {/*big*/ pa[hi] = sgn(neg[hi],1.0);}
        } else {
            if (two>big) {/*two*/
                pa[hi] = sgn(neg[hi],nv_sqrthalf); pa[lo] = sgn(neg[lo], nv_sqrthalf);
                if (lo>hi) {hi ^= lo; lo ^= hi; hi ^= lo;}
                if (hi==W) {hi = "\001\002\000"[lo]; lo = 3-hi-lo;}
                swap(ka,hi,lo)
            } else {/*big*/ pa[hi] = sgn(neg[hi],1.0);}
        }
        p.x = -pa[0]; p.y = -pa[1]; p.z = -pa[2]; p.w = pa[3];
    }
    k = ka;
    return (p);
}

/* Decompose Affine Matrix 
 *    A = TQS, where
 * A is the affine transform
 * T is the translation vector
 * Q is the rotation (quaternion)
 * U is the rotation component of the scaling factor (shear = U * S)
 * S is the scale vector
 * f is the sign of the determinant
 *
 * See Ken Shoemake and Tom Duff. Matrix Animation and Polar Decomposition.
 * Proceedings of Graphics Interface 1992.
 */
void decomp_affine(const mat4 & A, vec3 & T, quat & Q, quat & U, vec3 & S, nv_scalar & f)
{
    mat3    matQ, matS, matU, matM;
    quat    p;
    
    nv_scalar det;
    A.get_translation(T);
    A.get_rot(matM);
    det = polar_decomp(matM, matQ, matS);

    if (det < nv_zero)
    {
        negate(matQ);
        f = -nv_one;
    } 
    else 
        f = nv_one;

    mat_2_quat(Q, matQ);
    spect_decomp(S, matS, matU);
    mat_2_quat(U, matU);

    U *= snuggle(p, U, S);
    U *= p;
}

