#! /usr/bin/env python
#coding=utf-8

import unittest   #��Ԫ���Կ��
from test import support

#################################################################################
# Python.exe
#   file �ļ��� ��ָ���ļ�����python�ű�
#   �����в�����
#      -d �������
#      -O �����Ż����ֽ���
#      -v ��ϸ���
#      -m mod ��һ��ģ���Խű���ʽ����
#      -c cmd �������������ַ�����ʽ�ύ��python�ű�
#   �˳���Windows�� CTRL+Z�� Linux �� CTRL+D, help(quit)�� Ҳ��ͨ�� sys.exit() �˳�
#################################################################################

class ToolsTester(unittest.TestCase):
    '''Python Tools Tester'''   #�˴�д����Class��ע�ͣ�ͨ��__doc__�ɷ����ĵ��ַ���
    
    def setUp(self):
        print('in ToolsTester setUp')
        
    def tearDown(self):
        print("in ToolsTester tearDown")
        
    def test_Tools(self):
        #os.path.exists
        self.assertEqual(1+2,3)
        
def suite():
    suite = unittest.makeSuite(ToolsTester,'test')
    return suite

if __name__ == "__main__":
    unittest.main()        