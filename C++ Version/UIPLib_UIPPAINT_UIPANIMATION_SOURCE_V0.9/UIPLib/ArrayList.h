#ifndef __ARRAY_LIST_H
#define __ARRAY_LIST_H
#include "Base.h"
template<typename TYPE> class CArrayList
{
public:
	CArrayList()
	{
		m_pData = NULL; m_nSize = 0; m_nMaxSize = 0;
	}

	CArrayList( const CArrayList <TYPE>& a )
	{
		for( int i = 0; i < a.m_nSize; i++ ) Add( a.m_pData[i] );
	}

	~CArrayList()
	{
		RemoveAll();
	}

	const TYPE& operator[]( int nIndex ) const
	{
		return GetAt( nIndex );
	}

	TYPE& operator[]( int nIndex )
	{
		return GetAt( nIndex );
	}

	CArrayList& operator=( const CArrayList <TYPE>& a )
	{
		if( this == &a ) 
			return *this; 
		RemoveAll();
		for( int i = 0; i < a.m_nSize;i++ ) 
			Add( a.m_pData[i] );
		return *this;
	}

	bool SetSize( int nNewMaxSize );

	bool Add( const TYPE& value );

	bool Insert( int nIndex, const TYPE& value );

	bool SetAt( int nIndex, const TYPE& value );

	TYPE& GetAt( int nIndex ) const
	{
		assert( nIndex >= 0 && nIndex < m_nSize ); return m_pData[nIndex];
	}

	int     GetSize() const
	{
		return m_nSize;
	}

	TYPE* GetData()
	{
		return m_pData;
	}

	bool    Contains( const TYPE& value )
	{
		return ( -1 != IndexOf( value ) );
	}

	int     IndexOf( const TYPE& value )
	{
		return ( m_nSize > 0 ) ? IndexOf( value, 0, m_nSize ) : -1;
	}

	int     IndexOf( const TYPE& value, int iStart )
	{
		return IndexOf( value, iStart, m_nSize - iStart );
	}

	int     IndexOf( const TYPE& value, int nIndex, int nNumElements );

	int     LastIndexOf( const TYPE& value )
	{
		return ( m_nSize > 0 ) ? LastIndexOf( value, m_nSize - 1, m_nSize ) : -1;
	}

	int     LastIndexOf( const TYPE& value, int nIndex )
	{
		return LastIndexOf( value, nIndex, nIndex + 1 );
	}

	int     LastIndexOf( const TYPE& value, int nIndex, int nNumElements );

	bool Remove( int nIndex );

	void    RemoveAll()
	{
		SetSize( 0 );
	}

	void    Clear()
	{
		m_nSize = 0;
	}

protected:
	bool SetSizeInternal( int nNewMaxSize );

protected:
	TYPE* m_pData;      
	int m_nSize;        
	int m_nMaxSize;     
};

template<typename TYPE> bool CArrayList <TYPE>::SetSizeInternal( int nNewMaxSize )
{
	if( nNewMaxSize < 0 || ( nNewMaxSize > INT_MAX / sizeof( TYPE ) ) )
	{
		assert( false );
		return false;
	}

	if( nNewMaxSize == 0 )
	{
		if( m_pData )
		{
			free( m_pData );
			m_pData = NULL;
		}

		m_nMaxSize = 0;
		m_nSize = 0;
	}
	else if( m_pData == NULL || nNewMaxSize > m_nMaxSize )
	{

		int nGrowBy = ( m_nMaxSize == 0 ) ? 16 : m_nMaxSize;

		if( ( UINT )m_nMaxSize + ( UINT )nGrowBy > ( UINT )INT_MAX )
			nGrowBy = INT_MAX - m_nMaxSize;

		nNewMaxSize = __max( nNewMaxSize, m_nMaxSize + nGrowBy );

		if( sizeof( TYPE ) > UINT_MAX / ( UINT )nNewMaxSize )
			return false;

		TYPE* pDataNew = ( TYPE* )realloc( m_pData, nNewMaxSize * sizeof( TYPE ) );
		if( pDataNew == NULL )
			return false;

		m_pData = pDataNew;
		m_nMaxSize = nNewMaxSize;
	}

	return true;
}


//--------------------------------------------------------------------------------------
template<typename TYPE> bool CArrayList <TYPE>::SetSize( int nNewMaxSize )
{
	int nOldSize = m_nSize;

	if( nOldSize > nNewMaxSize )
	{
		assert( m_pData );
		if( m_pData )
		{

			for( int i = nNewMaxSize; i < nOldSize; ++i )
				m_pData[i].~TYPE();
		}
	}

	bool hr = SetSizeInternal( nNewMaxSize );

	if( nOldSize < nNewMaxSize )
	{
		assert( m_pData );
		if( m_pData )
		{
			for( int i = nOldSize; i < nNewMaxSize; ++i )
				::new ( &m_pData[i] ) TYPE;
		}
	}

	return hr;
}


//--------------------------------------------------------------------------------------
template<typename TYPE> bool CArrayList <TYPE>::Add( const TYPE& value )
{
	bool hr;
	if( (hr = SetSizeInternal( m_nSize + 1 )) == false )
		return hr;

	assert( m_pData != NULL );

	::new ( &m_pData[m_nSize] ) TYPE;
	m_pData[m_nSize] = value;
	++m_nSize;

	return true;
}


//--------------------------------------------------------------------------------------
template<typename TYPE> bool CArrayList <TYPE>::Insert( int nIndex, const TYPE& value )
{
	bool hr;

	if( nIndex < 0 ||
		nIndex > m_nSize )
	{
		assert( false );
		return false;
	}

	if( ( hr = SetSizeInternal( m_nSize + 1 ) ) == false )
		return hr;

	MoveMemory( &m_pData[nIndex + 1], &m_pData[nIndex], sizeof( TYPE ) * ( m_nSize - nIndex ) );

	::new ( &m_pData[nIndex] ) TYPE;

	m_pData[nIndex] = value;
	++m_nSize;

	return true;
}

template<typename TYPE> bool CArrayList <TYPE>::SetAt( int nIndex, const TYPE& value )
{
	if( nIndex < 0 ||
		nIndex >= m_nSize )
	{
		assert( false );
		return false;
	}

	m_pData[nIndex] = value;
	return true;
}

template<typename TYPE> int CArrayList <TYPE>::IndexOf( const TYPE& value, int iStart, int nNumElements )
{
	if( iStart < 0 ||
		iStart >= m_nSize ||
		nNumElements < 0 ||
		iStart + nNumElements > m_nSize )
	{
		assert( false );
		return -1;
	}

	for( int i = iStart; i < ( iStart + nNumElements ); i++ )
	{
		if( value == m_pData[i] )
			return i;
	}

	return -1;
}

template<typename TYPE> int CArrayList <TYPE>::LastIndexOf( const TYPE& value, int iEnd, int nNumElements )
{
	if( iEnd < 0 ||
		iEnd >= m_nSize ||
		nNumElements < 0 ||
		iEnd + 1 -  nNumElements < 0 )
	{
		assert( false );
		return -1;
	}

	for( int i = iEnd; i > ( iEnd - nNumElements ); i-- )
	{
		if( value == m_pData[i] )
			return i;
	}

	return -1;
}


template<typename TYPE> bool CArrayList <TYPE>::Remove( int nIndex )
{
	if( nIndex < 0 ||
		nIndex >= m_nSize )
	{
		assert( false );
		return false;
	}

	m_pData[nIndex].~TYPE();

	MoveMemory( &m_pData[nIndex], &m_pData[nIndex + 1], sizeof( TYPE ) * ( m_nSize - ( nIndex + 1 ) ) );
	--m_nSize;

	return true;
}
#endif