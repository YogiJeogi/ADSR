#ifndef __CMATRIX_H__
#define __CMATRIX_H__


//#define _USESTDVECTOR_

/*------------------------------------------------------------------
               Beginning of VECTOR Class
------------------------------------------------------------------*/
#ifdef _USESTDVECTOR_
#define NRvector vector
#else

template <class T>
class NRvector {
private:
	int nn;	// size of array. upper index is nn-1
	T *v;
public:
	NRvector();
	explicit NRvector(int n);		// Zero-based array
	NRvector(int n, const T &a);	//initialize to constant value
	NRvector(int n, const T *a);	// Initialize to array
	NRvector(const NRvector &rhs);	// Copy constructor
	NRvector & operator=(const NRvector &rhs);	//assignment
	NRvector(int n,double delta,bool isincrement); // f=0:delta:n*delta; 
	typedef T value_type; // make T available externally
	inline T & operator[](const int i);	//i'th element
	inline const T & operator[](const int i) const;
	inline int size() const;
	void resize(int newn); // resize (contents not preserved)
	void assign(int newn, const T &a); // resize and Assign a constant value
	void assignVectorOnRange(int ln,int un,const NRvector &rhs);
	void assignElementOnRange(int ln,int un,T nelement);
	void insertElementFront(int ielement,T nelement);
    void insertElementRear(int ielement,T nelement(T));
	void insertVectorFront(const NRvector &rhs);
    void insertVectorRear(const NRvector &rhs);
	void removeIndexElement(int ielement);
	void multiScalar(T nscalar);
	void divideScalar(T nscalar, bool isNumerate); // 분자이면 nscalar/element 계산
	void powerElementAlpha(T nAlpha);
	void sqrtElement();
	void flipup();
	double sumAllElement();

	~NRvector();
};
/*------------------------------------------------------------------
               END of VECTOR Class
------------------------------------------------------------------*/
/*------------------------------------------------------------------
               Beginning of MATRIX Class
------------------------------------------------------------------*/
template <class T>
class NRmatrix {
private:
	int nn;
	int mm;
	T **v;
public:
	NRmatrix();
	NRmatrix(int n, int m);			// Zero-based array
	NRmatrix(int n, int m, const T &a);	//Initialize to constant
	NRmatrix(int n, int m, const T *a);	// Initialize to array
	NRmatrix(const NRmatrix &rhs);		// Copy constructor
	NRmatrix & operator=(const NRmatrix &rhs);	//assignment
	NRmatrix & operator+(const NRmatrix &rhs);  // Handong
	typedef T value_type; // make T available externally
	inline T* operator[](const int i);
	inline const T* operator[](const int i) const;
	inline int nrows() const;
	inline int ncols() const;
	void resize(int newn, int newm); // resize (contents not preserved)
	void Assign(int newn, int newm, const T &a); // resize and assign a constant value
	void assignVector(int irow, int icol, const NRvector<T> &a); // HDL
	~NRmatrix();
};

/*------------------------------------------------------------------
               END of MATRIX Class
------------------------------------------------------------------*/
/*------------------------------------------------------------------
               Beginning of 3D MATRIX Class
------------------------------------------------------------------*/
template <class T>
class NRMat3d {
private:
	int nn;
	int mm;
	int kk;
	T ***v;
public:
	NRMat3d();
	NRMat3d(const NRMat3d &rhs);		// Copy constructor
	NRMat3d(int n, int m, int k);
	NRMat3d & operator=(const NRMat3d &rhs);	//assignment
	T** operator[](const int i);	//subscripting: pointer to row i
	const T* const * operator[](const int i) const;
	int dim1() const;
	int dim2() const;
	int dim3() const;
	void resize(int newn, int newm,  int newk); // resize (contents not preserved)
	void AssignConstant(int newn, int newm, int newk, const T& a);
	void AssignMatrixTOThirdIndex( int kindex, const NRmatrix<T> &a );
	void DivideElementBy(float den);

	~NRMat3d();
};
/*------------------------------------------------------------------
              END of 3D MATRIX Class
------------------------------------------------------------------*/



/*------------------------------------------------------------------
               BEGINNING of Vector Class
------------------------------------------------------------------*/
template <class T>
NRvector<T>::NRvector() : nn(0), v(NULL) {}

template <class T>
NRvector<T>::NRvector(int n) : nn(n), v(n>0 ? new T[n] : NULL) {}

template <class T>
NRvector<T>::NRvector(int n, const T& a) : nn(n), v(n>0 ? new T[n] : NULL)
{
	for(int i=0; i<n; i++) v[i] = a;
}

template <class T>
NRvector<T>::NRvector(int n, const T *a) : nn(n), v(n>0 ? new T[n] : NULL)
{
	for(int i=0; i<n; i++) v[i] = *a++;
}

template <class T>
NRvector<T>::NRvector(const NRvector<T> &rhs) : nn(rhs.nn), v(nn>0 ? new T[nn] : NULL)
{
	for(int i=0; i<nn; i++) v[i] = rhs[i];
}

template <class T>
NRvector<T> & NRvector<T>::operator=(const NRvector<T> &rhs)
// postcondition: normal assignment via copying has been performed;
//		if vector and rhs were different sizes, vector
//		has been resized to match the size of rhs
{
	if (this != &rhs)
	{
		if (nn != rhs.nn) {
			if (v != NULL) delete [] (v);
			nn=rhs.nn;
			v= nn>0 ? new T[nn] : NULL;
		}
		for (int i=0; i<nn; i++)
			v[i]=rhs[i];
	}
	return *this;
}

template <class T>
inline T & NRvector<T>::operator[](const int i)	//subscripting
{
#ifdef _CHECKBOUNDS_
if (i<0 || i>=nn) {
	throw("NRvector subscript out of bounds");
}
#endif
	return v[i];
}

template <class T>
inline const T & NRvector<T>::operator[](const int i) const	//subscripting
{
#ifdef _CHECKBOUNDS_
if (i<0 || i>=nn) {
	throw("NRvector subscript out of bounds");
}
#endif
	return v[i];
}

template <class T>
inline int NRvector<T>::size() const
{
	return nn;
}

template <class T>
void NRvector<T>::resize(int newn)
{
	if (newn != nn) {
		if (v != NULL) delete[] (v);
		nn = newn;
		v = nn > 0 ? new T[nn] : NULL;
	}
}

template <class T>
void NRvector<T>::assign(int newn, const T& a)
{
	if (newn != nn) {
		if (v != NULL) delete[] (v);
		nn = newn;
		v = nn > 0 ? new T[nn] : NULL;
	}
	for (int i=0;i<nn;i++) v[i] = a;
}

template <class T>
NRvector<T>::~NRvector()
{
	if (v != NULL) delete[] (v);
}

// end of NRvector definitions
/*------------------------------------------------------------
/	               HDL Functions
/------------------------------------------------------------*/
template <class T>
NRvector<T>::NRvector(int n,double delta,bool isincrement) : nn(n), v(n>0 ? new T[n] : NULL)
{
	for(int i=0; i<n; i++) v[i] = delta*i;
}

template <class T>
void NRvector<T>::assignVectorOnRange(int ln,int un,const NRvector &rhs)
{
#ifdef _CHECKBOUNDS_
if (un<0 || un>=nn) {
	throw("NRvector assignOnRange subscript out of bounds");
}
#endif

	for(int i=ln; i<un; i++) v[i] = rhs[i-ln];
}

template <class T>
void NRvector<T>::assignElementOnRange(int ln,int un,T nelement)
{
#ifdef _CHECKBOUNDS_
if (un<0 || un>=nn) {
	throw("NRvector assignOnRange subscript out of bounds");
}
#endif

	for(int i=ln; i<un; i++) v[i] = nelement;
}

template <class T>
void NRvector<T>::insertElementFront(int ielement,T nelement)
{
#ifdef _CHECKBOUNDS_
/*if (un<0 || un>=nn) {
	throw("NRvector assignOnRange subscript out of bounds");
}*/
#endif
	nn+=ielement;
	T *pVt=new T[nn];
    for (int i=0 ; i < ielement ; i++) pVt[i]=nelement;
	for (int i=ielement ; i< nn ; i++) pVt[i]=v[i-ielement]; 
	delete[] (v);
	v=pVt;
}

template <class T>
void NRvector<T>::insertElementRear(int ielement,T nelement(T))
{
#ifdef _CHECKBOUNDS_
/*if (un<0 || un>=nn) {
	throw("NRvector assignOnRange subscript out of bounds");
}*/
#endif

	nn+=ielement;
	T *pVt=new T[nn];
    for (int i=0 ; i < (nn-ielement) ; i++) pVt[i]=v[i];
	for (int i=(nn-ielement) ; i< nn ; i++) pvt[i]=nelement; 
	delete[] (v);
	v=pVt;
}


template <class T>
void NRvector<T>::insertVectorFront(const NRvector &rhs)
{
#ifdef _CHECKBOUNDS_
/*if (un<0 || un>=nn) {
	throw("NRvector assignOnRange subscript out of bounds");
}*/
#endif
	int nelement=rhs.size;
	nn=nn+nelement;
	T *pVt=new T[nn];
    for (int i=0 : i < nelement : i++) pVt[i]=rhs[i];
	for (int i=nelement : i< nn : i++) pvt[i]=v[i-nelement]; 
	delete[] (v);
	v=pVt;
}

template <class T>
void NRvector<T>::insertVectorRear(const NRvector &rhs)
{
#ifdef _CHECKBOUNDS_
/*if (un<0 || un>=nn) {
	throw("NRvector assignOnRange subscript out of bounds");
}*/
#endif
	int nelement=rhs.size();
	nn= nn+nelement;
	T *pVt=new T[nn];
    for (int i=0 : i < (nn-nelement) : i++) pVt[i]=v[i];
	for (int i=(nn-nelement) : i< nn : i++) pvt[i]=rhs[i-(nn-nelement)]; 
	delete[] (v);
	v=pVt;
}

template <class T>
void NRvector<T>::removeIndexElement(int ielement)
{
#ifdef _CHECKBOUNDS_
if (ielement<0 || ielement>=nn) {
	throw("NRvector removeIndexElement subscript out of bounds");
}
#endif

	nn = nn-1;
	for (int i=ielement:i<nn:i++)
	{
		v[i]=v[i+1];
	}
	v[nn]=0; // 마지막을 0으로 채운다. 기억 공간을 제거하지 않았기 대문에 메모리 손실이 있으나
	         // 계산상 빠르다.
}

template <class T>
void NRvector<T>::multiScalar(T nscalar)
{

	for(int i=0; i<nn; i++) v[i] = v[i]*nscalar;
}


template <class T>
void NRvector<T>::divideScalar(T nscalar, bool isNumerate)
{
	if(!isNumerate){
	for(int i=0; i<nn; i++) v[i] = v[i]/nscalar;
	}
	else {
		for(int i=0; i<nn; i++) {
			if (v[i]==0) v[i]=0;
			else v[i] = nscalar/v[i];
		}
	}
}

template <class T>
void NRvector<T>::powerElementAlpha(T nAlpha)
{

	for(int i=0; i<nn; i++) v[i] = pow(v[i],nAlpha);
}

template <class T>
void NRvector<T>::sqrtElement()
{
	for(int i=0; i<nn; i++) v[i] = sqrt(v[i]);
}

template <class T>
double NRvector<T>::sumAllElement()
{
	Doub Temsum=0;
	for(int i=0; i<nn; i++) Temsum +=v[i];
	return Temsum;
}
	 
template <class T>
void NRvector<T>::flipup()
{
	
		T *pVt=new T[nn];
		for (int i=0 : i <nn : i++) pVt[i]=v[nn-1-i];
		delete[] (v);
		v=pVt;
}

/*------------------------------------------------------------
/	               end of HDL Functions
/-------------------------------------------------------------*/
/*------------------------------------------------------------------
               End of Vector Class
------------------------------------------------------------------*/


#endif //ifdef _USESTDVECTOR_
/*------------------------------------------------------------------
               End of Vector Class
------------------------------------------------------------------*/

/*------------------------------------------------------------------
               Beginning of MATRIX Class
------------------------------------------------------------------*/

template <class T>
NRmatrix<T>::NRmatrix() : nn(0), mm(0), v(NULL) {}

template <class T>
NRmatrix<T>::NRmatrix(int n, int m) : nn(n), mm(m), v(n>0 ? new T*[n] : NULL)
{
	int i,nel=m*n;
	if (v) v[0] = nel>0 ? new T[nel] : NULL;
	for (i=1;i<n;i++) v[i] = v[i-1] + m;
}

template <class T>
NRmatrix<T>::NRmatrix(int n, int m, const T &a) : nn(n), mm(m), v(n>0 ? new T*[n] : NULL)
{
	int i,j,nel=m*n;
	if (v) v[0] = nel>0 ? new T[nel] : NULL;
	for (i=1; i< n; i++) v[i] = v[i-1] + m;
	for (i=0; i< n; i++) for (j=0; j<m; j++) v[i][j] = a;
}

template <class T>
NRmatrix<T>::NRmatrix(int n, int m, const T *a) : nn(n), mm(m), v(n>0 ? new T*[n] : NULL)
{
	int i,j,nel=m*n;
	if (v) v[0] = nel>0 ? new T[nel] : NULL;
	for (i=1; i< n; i++) v[i] = v[i-1] + m;
	for (i=0; i< n; i++) for (j=0; j<m; j++) v[i][j] = *a++;
}

template <class T>
NRmatrix<T>::NRmatrix(const NRmatrix &rhs) : nn(rhs.nn), mm(rhs.mm), v(nn>0 ? new T*[nn] : NULL)
{
	int i,j,nel=mm*nn;
	if (v) v[0] = nel>0 ? new T[nel] : NULL;
	for (i=1; i< nn; i++) v[i] = v[i-1] + mm;
	for (i=0; i< nn; i++) for (j=0; j<mm; j++) v[i][j] = rhs[i][j];
}

template <class T>
NRmatrix<T> & NRmatrix<T>::operator=(const NRmatrix<T> &rhs)
// postcondition: normal assignment via copying has been performed;
//		if matrix and rhs were different sizes, matrix
//		has been resized to match the size of rhs
{
	if (this != &rhs) {
		int i,j,nel;
		if (nn != rhs.nn || mm != rhs.mm) {
			if (v != NULL) {
				delete[] (v[0]);
				delete[] (v);
			}
			nn=rhs.nn;
			mm=rhs.mm;
			v = nn>0 ? new T*[nn] : NULL;
			nel = mm*nn;
			if (v) v[0] = nel>0 ? new T[nel] : NULL;
			for (i=1; i< nn; i++) v[i] = v[i-1] + mm;
		}
		for (i=0; i< nn; i++) for (j=0; j<mm; j++) v[i][j] = rhs[i][j];
	}
	return *this;
}
template <class T>
NRmatrix<T> & NRmatrix<T>::operator+(const NRmatrix<T> &rhs)
// Handong Lee
{
	int i,j;
	if (rhs.nn != nn || rhs.mm != mm) return *this;
		
	for (i=0; i< nn; i++) for (j=0; j<mm; j++) v[i][j] += rhs[i][j];
	return *this;
}

template <class T>
inline T* NRmatrix<T>::operator[](const int i)	//subscripting: pointer to row i
{
#ifdef _CHECKBOUNDS_
	if (i<0 || i>=nn) {
		throw("NRmatrix subscript out of bounds");
	}
#endif
	return v[i];
}

template <class T>
inline const T* NRmatrix<T>::operator[](const int i) const
{
#ifdef _CHECKBOUNDS_
	if (i<0 || i>=nn) {
		throw("NRmatrix subscript out of bounds");
	}
#endif
	return v[i];
}

template <class T>
inline int NRmatrix<T>::nrows() const
{
	return nn;
}

template <class T>
inline int NRmatrix<T>::ncols() const
{
	return mm;
}

template <class T>
void NRmatrix<T>::resize(int newn, int newm)
{
	int i,nel;
	if (newn != nn || newm != mm) {
		if (v != NULL) {
			delete[] (v[0]);
			delete[] (v);
		}
		nn = newn;
		mm = newm;
		v = nn>0 ? new T*[nn] : NULL;
		nel = mm*nn;
		if (v) v[0] = nel>0 ? new T[nel] : NULL;
		for (i=1; i< nn; i++) v[i] = v[i-1] + mm;
	}
}

template <class T>
void NRmatrix<T>::Assign(int newn, int newm, const T& a)
{
	int i,j,nel;
	if (newn != nn || newm != mm) {
		if (v != NULL) {
			delete[] (v[0]);
			delete[] (v);
		}
		nn = newn;
		mm = newm;
		v = nn>0 ? new T*[nn] : NULL;
		nel = mm*nn;
		if (v) v[0] = nel>0 ? new T[nel] : NULL;
		for (i=1; i< nn; i++) v[i] = v[i-1] + mm;
	}
	for (i = 0; i < nn; i++) for (j = 0; j < mm; j++) v[i][j] = a;
}

template <class T>
void NRmatrix<T>::assignVector(int irow, int icol,const NRvector<T> &a) // HDL
{
  // if ( irow >= nn || icol+a.size() >= mm ) throw(" ");
   
   for(int i=0 ; i < a.size() ; i++ ){
   v[irow][icol+i]=a[i];
   }
   
}

template <class T>
NRmatrix<T>::~NRmatrix()
{
	if (v != NULL) {
		delete[] (v[0]);
		delete[] (v);
	}
}


/*------------------------------------------------------------------
               END of MATRIX Class
------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////////////



/*------------------------------------------------------------------
               Beginning of 3D MATRIX Class
------------------------------------------------------------------*/

template <class T>
NRMat3d<T>::NRMat3d(): nn(0), mm(0), kk(0), v(NULL) {}

template <class T>
NRMat3d<T>::NRMat3d( const NRMat3d &rhs ) : nn(rhs.nn), mm(rhs.mm), kk(rhs.kk ), v( nn>0 ? new T**[nn] : NULL)
{
	int i,j,k;
	v[0] = new T*[nn*mm];
	v[0][0] = new T[nn*mm*kk];
	for(j=1; j<mm; j++) v[0][j] = v[0][j-1] + kk;
	for(i=1; i<nn; i++) {
		v[i] = v[i-1] + mm;
		v[i][0] = v[i-1][0] + mm*kk;
		for(j=1; j<mm; j++) v[i][j] = v[i][j-1] + kk;
	}
	for (i=0; i< nn; i++) for (j=0; j < mm; j++) for (k=0; k<kk; k++) v[i][j][k] = rhs[i][j][k];
}


template <class T>
NRMat3d<T>::NRMat3d(int n, int m, int k) : nn(n), mm(m), kk(k), v(new T**[n])
{
	int i,j;
	v[0] = new T*[n*m];
	v[0][0] = new T[n*m*k];
	for(j=1; j<m; j++) v[0][j] = v[0][j-1] + k;
	for(i=1; i<n; i++) {
		v[i] = v[i-1] + m;
		v[i][0] = v[i-1][0] + m*k;
		for(j=1; j<m; j++) v[i][j] = v[i][j-1] + k;
	}
}

template <class T>
T** NRMat3d<T>::operator[](const int i) //subscripting: pointer to row i
{
	return v[i];
}

template <class T>
const T* const * NRMat3d<T>::operator[](const int i) const
{
	return v[i];
}

template <class T>
int NRMat3d<T>::dim1() const
{
	return nn;
}

template <class T>
int NRMat3d<T>::dim2() const
{
	return mm;
}

template <class T>
int NRMat3d<T>::dim3() const
{
	return kk;
}

template <class T>
NRMat3d<T> & NRMat3d<T>::operator=(const NRMat3d<T> &rhs)
// postcondition: normal assignment via copying has been performed;
//		if matrix and rhs were different sizes, matrix
//		has been resized to match the size of rhs
{
	if (this != &rhs) {
		int i,j,k;
		if (nn != rhs.nn || mm != rhs.mm || kk !=rhs.kk ) {
			if (v != NULL) {
				delete[] (v[0][0]);
				delete[] (v[0]);
				delete[] (v);
			}
			nn=rhs.nn;
			mm=rhs.mm;
			kk=rhs.kk;
			v = nn>0 ? new T**[nn] : NULL;
			v[0] = new T*[nn*mm];
			v[0][0] = new T[nn*mm*kk];
			for(j=1; j<mm; j++) v[0][j] = v[0][j-1] + kk;
			for(i=1; i<nn; i++) {
				v[i] = v[i-1] + mm;
				v[i][0] = v[i-1][0] + mm*kk;
				for(j=1; j<mm; j++) v[i][j] = v[i][j-1] + kk;
			}


		}
		for (i=0; i< nn; i++) for (j=0; j<mm; j++) for (k=0; k<kk; k++) v[i][j][k] = rhs[i][j][k];
	}
	return *this;
}

template <class T>
void NRMat3d<T>::resize(int newn, int newm, int newk)
// Handong
{
	int i,j;
	if (newn != nn || newm != mm || newk !=kk) {
		if (v != NULL) {
			delete[] (v[0][0]);
			delete[] (v[0]);
			delete[] (v);
		}
		nn = newn;
		mm = newm;
		kk = newk;
		v = nn>0 ? new T**[nn] : NULL;
		v[0] = new T*[nn*mm];
		v[0][0] = new T[nn*mm*kk];
		for(j=1; j<mm; j++) v[0][j] = v[0][j-1] + kk;
		for(i=1; i<nn; i++) {
			v[i] = v[i-1] + mm;
			v[i][0] = v[i-1][0] + mm*kk;
			for(j=1; j<mm; j++) v[i][j] = v[i][j-1] + kk;
		}
	}
}

template <class T>
void NRMat3d<T>::AssignConstant(int newn, int newm, int newk,  const T& a)
// Han dong
{
	int i,j,k;
	if (newn != nn || newm != mm || newk !=kk) {
		if (v != NULL) {
			delete[] (v[0][0]);
			delete[] (v[0]);
			delete[] (v);
		}
		nn = newn;
		mm = newm;
		kk = newk;
		v = nn>0 ? new T**[nn] : NULL;
		v[0] = new T*[nn*mm];
		v[0][0] = new T[nn*mm*kk];
		for(j=1; j<mm; j++) {v[0][j] = v[0][j-1] + kk;}
		for(i=1; i<nn; i++) {
			v[i] = v[i-1] + mm;
			v[i][0] = v[i-1][0] + mm*kk;
			for(j=1; j<mm; j++) v[i][j] = v[i][j-1] + kk;
		}
	}
	for (i=0; i< nn; i++) for (j=0; j<mm; j++) for (k=0; k<kk; k++) v[i][j][k] = a;
}

template <class T>
void  NRMat3d<T>::AssignMatrixTOThirdIndex(int kindex, const NRmatrix<T> &a){
	int i,j;
	if ( v != NULL ) {
		if ( a.nrows() == nn && a.ncols() == mm ){
			for (i=0; i< nn; i++) for (j=0; j<mm; j++) v[i][j][kindex] = a[i][j];
		}
	}
}


template <class T>
void NRMat3d<T>::DivideElementBy( float den )
{
	int i,j,k;
	for (i=0; i< nn; i++) for (j=0; j<mm; j++) for (k=0; k<kk; k++) v[i][j][k] /= den;
}


template <class T>
NRMat3d<T>::~NRMat3d()
{
	if (v != NULL) {
		delete[] (v[0][0]);
		delete[] (v[0]);
		delete[] (v);
	}
}

/*------------------------------------------------------------------
              END of 3D MATRIX Class
----------------------------*/
#endif