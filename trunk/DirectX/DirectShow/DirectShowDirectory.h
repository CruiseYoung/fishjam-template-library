#ifndef DIRECT_SHOW_DIRECTORY_H
#define DIRECT_SHOW_DIRECTORY_H

/****************************************************************************************************
* ����ý���ʽ��Ŀ¼�ṹ
* HDD -- ���� Content > File �Ĺ�ϵ���棿
* 
* Blu-ray Disc(������̴洢��Ƶʱ�ĸ�ʽ��AVCHD)����ΪBD_ROM��BD_R(Write-Once)��BD_RE(Rewritable)
*   Root
*    +-BDMV\ -- �����Ӱ�ļ���
*      +-index.bdmv -- ���ڱ���BDMVĿ¼��������Ϣ�������� INDEX.BDM �������� .mpls �ļ���˳���50i/60i��ʶ����Ϣ��
*      +-MovieObject.bdmv -- �洢һ������"MovieObject"����Ϣ
*      +-AUXDATA\   -- ���������������ļ�
*        [+-sound.bdmv] -- ��ѡ�İ�����HDMV Interactive Graphic streams���������ļ�
*        [aaaaa.otf]    -- �洢"Text subtitle"��������Ϣ
*      +-BACKUP\    -- ����index.bdmv��MovieObject.bdmv��PLAYLIST��CLIPINFOĿ¼�µ������ļ������б���
*      +-CLIPINFO\  
*        +-00000.clpi   -- �洢���� Clip AV���ļ���Clip Infomation(��ConnectionCondition--��
*            ��Content���ӵ�����,PlayListMark��)���ļ�
*        +-xxxxx.clpi -- ��һ����˳�������
*      +-PLAYLIST\  -- ���������б�����ݿ��ļ�
*        +-00000.mpls   -- �洢Movie PlayLists����Ϣ��ÿ�������б��Ӧһ���ļ�
*        +-yyyyy.mpls
*      +-STREAM\    -- ����AV���ļ�(Importʱ������ xxxxx.modd �ļ�����ʲô��)
*        +-00000.m2ts   -- ���� BDAV MPEG-2 transport stream���� xxxxx.clpi ��Ӧ���������������ļ�
*        +-xxxxx.m2ts   -- (��NTFS��PC�࣬CC=1n6ʱ����һ��m2ts��n��clpi��)
*    +-DummySpace -- ռλ�ļ������ڱ�֤�������㹻�������(1G?)
*
* CD-ROM(CD-R, CD-RW)
* 
* CF(CompactFlash)
*   Root
*    +-SONYCF.IND
* 
* DCF
*   DCF Folder�����淶
*       1.����Ϊ8���ַ���
*       2.��1���ַ�Ϊ����1-9��
*       3.��2-3���ַ�Ϊ����0-9��
*       4.��4-8���ַ�0-9��A-Z��a-z��_��
*   DCF File�����淶
*       1.����Ϊ12���ַ�(������չ����
*       2.��1-4���ַ�Ϊ0-9��A-Z��a-z��_��
*       3.��5-8���ַ�Ϊ����0-9�����Ҳ���ȫΪ0��
*   DCFĿ¼��ŷ�ʽ
*       1.ȡ��/DCIM��/MP_ROOT�б�������ļ��м����ţ�a��
*       2.��1���ҵ����ļ�����ȡ�����ļ���ţ�b��
*       3.����ʱ�ļ��б��(c)=(a),��һ�������ļ����(d)=b+1
*         ʵ����(�Ƿ񲻱�׼��)��ԭ�ļ��������ɲ��ֿ��Բ��䣬���޸ı�š�eg: Cano0028.JPG\Betw0001.JPG
*       4.�ļ��������9999���ļ��б������999���ļ��в�����ʱ�������ļ��С��ļ���������ļ�����4000��
*       5.��ϵ���ʱ�ı�ŷ���(���յ���˳�򣬵������)��
*         /DCIM/101MSDCF/DSC00001.JPG     
*         /DCIM/101MSDCF/DSC00002.JPG     
*         /MP_ROOT/101ANV01/MAV00003.MP4     
*         /DCIM/101MSDCF/DSC00004.JPG
*   Root--
*    +-AVF_INFO\ -- AVIndex Ŀ¼
*      +-AVIN0001.INP -- Property ��Ϣ
*      +-AVIN0001.INT -- Thumbnail Data
*    +-MP_ROOT\  -- 
*      +-100ANV01\  -- MP4�ļ�
*        +-MA*xmmm.MP4
*        +-MAQxmmm.MP4
*    +-DCIM
*      +-100XXXnn\
*        +-XXXXnnnn.JPG -- JPEG Files
*        +-XXXXnnnn.MPG -- L-mode Files
*    
*  
* ���ֲ�ͬ�ķ��෽ʽ��
*   DVD����ʽ�� DVD-Video��DVD-VR��DVD+VR��AVCHD��BDAV��HDMV ��(�� enum DvdFormat )
*   ý��������DVD-ROM��DVD-R��DVD-RW��DVD+R��DVD+RW��DVD-RAM
*   ��8cm��12cm����ֱ��(diameter)�� DL(DoubleLayer˫�㣿)  
* DVD -- 
*   Root
*    +-AUDIO_TS\  -- Ԥ����DVD-AUDIO��Ŀ¼
*    +-VIDEO_TS\  -- �洢����DVD-VIDEO��������Ƶ����Ƶ����Ļ�ȣ��������ݵ�Ŀ¼ 
*      +-VTS_XX_X.VOB(Video OBjects) -- ��ƵĿ���ļ�������Ƶ����������Ļ���������
*      +-VTS_TS.IFO(InFOrmation) -- �����������Ϣ�ļ���
*          IFO�ļ���DVD���Ż��ṩ�����Ϣ�������½ڵĿ�ʼʱ�䣬����������Ļλ�õȡ�
*          VOB�ļ��ǵ�Ӱ��������Ӧ��IFO�ļ�����ɵ�Ӱ�ĸ���Ƭ���л�����������
*      +-VTS_XX_X.IFO -- ���������Ϣ�ļ������� ����_��� �ķ�ʽ��������������99�����⣬ÿ�����������10���ļ���
*      +-VTS_TS.BUP(BackUP)--��һ�������ļ���
*          BUP��IFO�ļ���ȫ��ͬ�����������IFO�ļ������ʱ��ʹ�õı����ļ���
*    +-VIDEO_RM\ -- �����DVD+VR�Ļ��и�Ŀ¼�����»���VIDEO_RM.DAT �� VIDEO_RM.IFO ���ļ���
*    +-DVD_RTAV\ -- �����DVD-VR�Ļ��и�Ŀ¼�����»���VR_MOVIE.VRO�ļ�
*
* HDDisc
*   Root
*    +-DiscInfo.ini     -- ��DDM��¼��
*    +-VideoFiles\
*      +-CtdHDPj.xml
*      +-PlayList.vzpl
* 
* MemoryStick(Memory Stick Video File Format) 
*   Root
*     +-MEMSTICK.IND
* 
*****************************************************************************************************/

#endif //DIRECT_SHOW_DIRECTORY_H