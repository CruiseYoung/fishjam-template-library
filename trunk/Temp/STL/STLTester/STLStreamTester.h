#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*******************************************************************************************************************
* << (������) -- �������������
* >> (��ȡ��) -- ��������������
* ������/�������Ϊ�������� -- �κν��ܵ������������㷨�����Ժ���һ����
*
* ios_base
*   <== basic_ios -- ios<char>, ���ж����˴򿪷�ʽ,�� app/ate/binary/in/out ��
*     <== basic_istream -- istream<char>
*       <== basic_ifstream -- ifstream<char>,���ļ����� 
*       <== basic_istringstream -- istringstream<char>
*     <== basic_ostream -- ostream<char>
*       <== basic_ofstream -- ofstream<char>, ������ļ�
*       <== basic_ostringstream -- ostringstream<char>
*       <== ostrstream -- �Ѿ�����̭��(ʹ��C��ʽ���ַ���,����char*,�����Լ��ͷ��ڴ�),����� ostringstream 
*
*      <== basic_iostream(ͬʱ�� basic_istream �� basic_ostream �̳�,ͬʱ֧������/���) -- iostream<char>
*         <== basic_fstream -- fstream<char>
*         <== basic_stringstream -- stringstream<char>
*         <== strstream<char>
*
* basic_streambuf -- streambuf<char>
* basic_stringbuf -- stringbuf<char>
*
* eof -- ����Ƿ񵽴��ļ�β
*
* C++ I/Oϵͳ����������һ���ļ�����ϵ��ָ�룬ÿ��ִ����������ʱ����Ӧ��ָ���Զ��仯��
*   ��ָ�� -- ����������ļ��е�λ��
*   дָ�� -- �´�д������λ��
*******************************************************************************************************************/


/*******************************************************************************************************************
*
* ifstream in("name.txt", ios::binary|ios::in, ���ļ�����[0-��ͨ,1-ֻ��,2-����,4-ϵͳ]); -- ���ļ�
* while(getline(in, strtmp, '\n'))  //ѭ����ȡÿһ��
*   vect.push_back(strtmp.substr(0, strtmp.find(' '))); ��ÿһ���пո���ǰ����Ϣ�ŵ� vector<string>��
*   //copy(istream_iterator<string>(ifs), istream_iterator<string>(), back_inserter(vec_str));
* copy(vect.begin(), vect.end(), ostream_iterator<string>(cout, "\n"));  //�������Ļ�� -- ��ôȥ�����һ������?
* 
* �������� cout ����ĸ�ʽ(��16�������hex,10�������dec,8�������oct), endl-����, ends-���ַ�
*   setpxecision(n) -- ���ø������ľ���λ�����, �� cout << setpxecision(5) << PI 
*   cout.setf(ios::hex,ios::basefield);
*   cout<<"hr="<<hr<<endl;  //���� cout << std::hex << hr
*
* ʹ�ÿ��ַ��� wcout
* 
* ��ʽ��
*   cout << setiosflags(ios::left) << setw(15) << "CString:" << setw(50) << strResult.GetBuffer(0) << endl;
*   cout << setiosflags(ios::left) << setw(15) << "LPCTSTR:" << setw(50) << szResult << endl;
*   cout << setiosflags(ios::left) << setw(15) << "LPCTSTR1:" << setw(50) << szResult1 << endl;
*
* �����ƶ�д:
*   put/get
*   read -- ͨ�� gcount ȡ��ʵ�ʶ�ȡ���ַ���
*   write
*   seekg -- ��λ��ָ��
*   seekp -- ��λдָ��
*   
* ע�⣺
*   1.ostringstream ��Ȼ�ڸ�ʽ�����ʱ�ܲ�������Ϊ�乹�캯���л���� locale �Ĺ��죬���ȫ����������ڶ��̻߳����£�
*     ���Ƶ������ ostringstream ��ʵ������������ܵļ����½� -- ���˻��� snprintf ���⣬���б�ķ������Ƿ���Գ������� ostringstream
*     basic_istringstream => locale::locale => pthread_mutex_lock
*   2.ostringstream �� str().c_str() ���ص�����ʱ���󣬱��ʽ��β��������c_str() ���صĽ���Ұָ�룬
*     Ӧ�ñ�����ʱ����Ȼ�����ʹ��: const string& strTemp = ostream.str(); const char* cValue = strTemp.c_str();
*     
*******************************************************************************************************************/
class CSTLStreamTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLStreamTester );
    CPPUNIT_TEST( test_ifstream );
    CPPUNIT_TEST( test_istreambuf_iterator );
    CPPUNIT_TEST( test_ostringstream );
    CPPUNIT_TEST( test_ostrstream );
    CPPUNIT_TEST_SUITE_END();

    void test_ifstream();

    //��Ҫһ��һ���ַ�����ʱ����ʹ�� istreambuf_iterator -- Effective STL �Ĺ���29
    void test_istreambuf_iterator();

    void test_ostringstream();

    //���� str ������������ freeze(false) ���������������ڴ�й© -- ������ ostringstream 
    void test_ostrstream();

    DECLARE_DEFAULT_TEST_CLASS(CSTLStreamTester);
};