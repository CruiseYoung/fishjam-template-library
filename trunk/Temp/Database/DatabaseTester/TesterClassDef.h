#ifndef _FJUTILITY_TEST_TESTER_CLASS_DEF_H
#define _FJUTILITY_TEST_TESTER_CLASS_DEF_H

#pragma once

//����Ĭ�ϵĲ�����
//  1.�յ�Ĭ�Ϲ��졢Ĭ������
//  2.˽�еĿ�������͸��Ʋ�����
//  3.�յ� CPPUNIT���غ��� setUp �� tearDown

#define DECLARE_DEFAULT_TEST_CLASS(className) \
public:\
	className(){};\
	virtual ~className(){};\
public:\
	virtual void setUp(){};\
	virtual void tearDown(){};\
private:\
	className( const className &copy );\
	void operator =( const className &copy );

#endif //_FJUTILITY_TEST_TESTER_CLASS_DEF_H