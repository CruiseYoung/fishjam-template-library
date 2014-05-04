#include "StdAfx.h"
#include "STLStreamTester.h"

#include <string>
#include <vector>
#include <fstream>
#include <strstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CSTLStreamTester::test_ifstream()
{
    const char* fileNames[] = 
    {
        //������Windows��ʽ����Unix��ʽ�������һ��
        "ReadMe.txt",
        "ReadMe_Unix.txt"
    };
    for (int i = 0; i < _countof(fileNames) ; ++i)
    {
        std::string strtmp;
        //std::vector<std::string> vect;
        int TotalLineCount = 0;
        int NotEmptyCount = 0;
        std::ifstream in(fileNames[i]);//���ļ�

        while(getline(in, strtmp, '\n'))  //ѭ����ȡÿһ��
        {
            ++TotalLineCount;
            if (!strtmp.empty())
            {
                ++NotEmptyCount;
            }

            // ��ÿһ���пո���ǰ����Ϣ�ŵ� vector<string>��
            //vect.push_back(strtmp.substr(0, strtmp.find(' ')));
        }
        //ReadMe.txt �ļ������� -- ע�����һ������ǿ��еĻ������ᱻͳ��(��û�����һ��һ��)
        CPPUNIT_ASSERT(93 == TotalLineCount);
        CPPUNIT_ASSERT(68 == NotEmptyCount);

        //������������25��
        CPPUNIT_ASSERT(25 + NotEmptyCount == TotalLineCount);

        //�������Ļ��
        //std::copy(vect.begin(), vect.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    }
}

void CSTLStreamTester::test_ostringstream()
{
    std::ostringstream os;
    os << "Hello" << " World " << 100;
    std::string s = os.str();
    CPPUNIT_ASSERT(s == "Hello World 100");
}

void CSTLStreamTester::test_istreambuf_iterator()
{
    //��һ���ı��ļ�������һ���ַ���������
    std::ifstream inputFile("STLStreamTester.h");
    
    //istream_iterator<char>����ʹ��operator>>�����������ж�ȡ�����ַ���
    //istreambuf_iterator<char>����������Ļ�������ֱ�Ӷ�ȡ��һ���ַ���
    //����һ��istreambuf_iterator<char> �����һ��istream s�ж�ȡ�����s.rdbuf()->sgetc()����s����һ���ַ�

#if 0
    inputFile.unset(ios::skipws);				// �ر�inputFile�ĺ��Կո��־ -- ���Զ���ո�
    std::string fileData((istream_iterator<char>(inputFile)), istream_iterator<char>());    //���ܲ�
#endif
    //istreambuf_iterator ����Ҫ���ÿո��־
    std::string fileData((std::istreambuf_iterator<char>(inputFile)),std::istreambuf_iterator<char>()); //���ܺ�

    TRACE("**********************************************************************\n");
    TRACE("%s\n",fileData.c_str());
    TRACE("**********************************************************************\n");
}

void CSTLStreamTester::test_ostrstream()
{

}