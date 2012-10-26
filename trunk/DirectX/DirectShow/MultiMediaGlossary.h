
#ifndef MULTI_MEDIA_GLOSSARY_H
#define MULTI_MEDIA_GLOSSARY_H
//��ý��������� --  http://blog.csdn.net/hhhbbb/article/details/7619857

/******************************************************************************************************
* ����Ƶ�����������
*   ����ͬ�Ĳ������£���¼��Ƶ����ϢԶ�ȸ�Ƶ����ϸ(��Ƶʱÿ���񶯲����Ĵ����ȸ�Ƶʱ��)��
*   �ϸߵĲ�����ֻ�����ģ���źŵ�ʱ������ã����������ź�����(����48KHz��44.1KHz��CD�����޷��������,������ͬ��44.1KHzЧ�����)
*   �˶�������������Ƶ�ʽ��� 20Hz~20KHz ֮�䣬�����������ľ���4KHz����Ƶ����ʱͨ�����˺ܶ���������������
*   �Լ���Ϣ���Ӷȣ�����ѹ���� -- ���ݡ��˶���������ģ�͡�������VBR�������б���
*
******************************************************************************************************/

/******************************************************************************************************
* 1080/50i -- ???����ԭ�����Ǹ��е�,N��60HZ,����60I��,P��50HZ,����50I
* 
* 1080i -- ���ø���(interlace)ɨ�輼���ĸ����񣬲���2:1����ɨ�裬��1920��1080���ص�һ֡�ֳ�����ɨ�衣
* 1080p -- ����1080ɨ�裬ÿ֡���涼��1080�ߡ�1080pģʽ����HDMI�������ʱ��ʵ�֡�
* 1080/50i -- �й�������ӵ������Ͳ�����׼��1080/24P Ϊ��Ŀ�����ͽ�����׼��
* 24P TRUE CINEMA -- ��Ӱ����ÿ��24֡������ģ����ⲥ��������֧��24֡���ţ������ܵ��·������Ժ���Ӱ�����顣
* 720p  -- ��������(progressive)ɨ�輼���ĸ����񣬰�1280x720���ر���
* AACS(Advanced Access Content System) -- (����)��Ȩ���������𱣻���������ݣ��������Ȩ��������Ҫ��һ��
* ABR(Average Bitrate) -- ƽ�������ʣ���VBR��һ�ֲ�ֵ������LAME���CBR���ѵ��ļ�����Ⱥ�VBR�����ļ���С�������ص���������ֱ���ģʽ��
*   ��ָ�����ļ���С�ڣ���Ƶ�Ͳ�����Ƶ��ʹ����Ե͵���������Ƶ�ʹ�̬����ʱʹ�ø���������ΪVBR��CBR��һ������ѡ��
* ADSL(Asymmetrical Digital Subscriber Line) -- �ǶԳ������û��ߡ�
* ASF(Advanced Streaming Format)--�߼���ý���ʽ��΢�����ý���ļ����ͣ�ֻ��һ������������û�й涨����Ƶ��ѹ����ʽ����ASF�ļ��У�
*   ���԰����κθ�ʽ��ѹ��(����MPEG-4)���ѹ�������ݡ���Ҫʹ��ר�ŵ�Windows Media Format SDK ������
*   ������ɵ�Windows Media �ļ�ֻ������Ƶ��һ��ʹ��.wma��չ���������������Ƶ��һ��ʹ��.wmv��չ��
* ASP(Advanced Simple Profile) -- [MPEG4]�߼�����ܣ������� Baseline/Main/High �ȶ���?
* AVCHD(Advanced Video Codec High Definition) -- ���������¼����Ӱ���һ�����ͨ���ļ�ϵͳ��Ӧ���ָ�����ý�塣
*   ����BD��DVD��¼ʱBitrate�����ޣ�BD 24Mbps ���£�DVD 18Mbps ���¡�
* AVI -- 
* AVIndex(?) --����Sony��DSC��HandyCam�����Contents��Database��
*   �������ݷ�Ϊ Content(AVCHD HD��MPEG2 SD��DCF��ֹ���� MPLS ) �� PlayList(AVCHD HD��MPEG2 SD)��
*   ���Ա��� �û���Ϣ(�ղء�ShareMark��Event) �� ϵͳ��Ϣ(Ц����С������д1~N)
*   DSC����ֹ��(JPEG)������(MPEGX,MP4[AVC/M4V])��������˵Ļ���Set����ʱ���Զ��޸���
*   HandyCam����AVCHD(MPEG2-TS(AVC))��SD(MPEG2-PS)����ʹ���ˣ�Ҳ�����Զ��޸�������� Salvage(��) �Ĺ��ܡ�
*   �������ļ���AVF_INFO�ļ����е�AVIN0001.INP(Property��Ϣ)��AVIN0001.BNP��AVIN0001.INT(Thumbnail Data)
* BD-J -- 
* BD+ -- һ��΢�������������������������������Ȩ�����������������Բ�����������
* BDAV -- 
* BDMV(Blu-ray Disc Movie)--������̴洢��Ƶʱ�ĸ�ʽ��Ҳ����BD-Video��
* Blu-ray Disc����������Ӣ�ļ��BD�� -- ��SONY�����µ�������ҵ��ɵ�"����������"��Blu-ray Disc Association,
*   ǰ���� Blu-ray Disc Founders -- BDF���߻��Ĵ����������񡣲��ò���405����(nm)����ɫ������������ж�д������
*   ������25 GB (����)��50 GB (˫��)��100 GB (�Ĳ�)��200 GB (�˲�)��
*   ������Java����(BD-J)��������������������������ʵ�ָ��ӵĽ���ʽ���ܡ�
*   ��������Ҫ��3�ַ�ʽ������λ��Ȩ����DRM��: AACS��BD+��ROM Mark��
*   ������ʹ��CDFS��ʽ��������������¼��д�����ϣ���ΪBD-R��������¼����BD-RE�������¼����ʽ��
* BRAVIA -- 
* Bus -- �������ͣ�Ŀǰ��ATAPI,SCSI,IEEE1394,USB,USB2 �ȼ��֡�
* CBR(Constants Bit Rate) -- �̶����ʣ��ļ���ͷ��β����һ��λ���ʣ��ļ�����ܴ󣬵����������VBR��ABR���������Ե���ߡ�
* CC(Closed Caption) -- ���صĴ��н�����ζ����Ļ, IAMAnalogVideoEncoder �ӿڿ���
* CC(ConnectionCondition) -- AVCHD ��ʹ�õģ���������Content���ӵ�������ͨ��ö�� CCType ���б�ʾ(������PlayItem�У�)����Ϊ��
*   CC1(һ�����ָ���Ѿ��ֿ���ý��mts����PC��Set��--ʱ��Ƚ϶̣�����Ҫ�ָ)
*   CC1n6(һ���Ѿ��ϲ���M2TS����PC�ϣ���Ҫ��NTFS��ʽ)
*   CC6(��FAT32��Set�ϣ��ָ���Ķ��С��2G���ļ�)
*   �� PlayListMark �б��棬���л��� clpi��PlayItem ����Ϣ���������������ֶ��ָ����ز�ʱ����Ҫ���м��С�
*   �ָ��ز�ʱʹ�ð�����Clpi�е� EpMap(pts �� spn ���б�) ����Ϣ���ָ�ʱֻ�ܰ� GOP ��λ���У��ӹ����˵Size������6144Byte�ı�����
*   �ָ�㵽EPMap�߽�֮����Null Packet(��Header����ȫ��0xFF��Packet)���ǡ�
* CD -- ����780���ײ���
* ConvertedContent -- ����ʵ�ʽ���д���ش�����࣬(��Ŀ�ģ�)
* CUDA(Compute Unified Device Architecture) -- �Կ�����NVidia�Ƴ�������ƽ̨��
*   nVidia��H.264/AVC Ӳ��GPU����? ������ DXVA? ATI ���� OpenCL? Intel������CPU����Quick Sync Video?
* DCF(Design rule for Camera File system)--���������ͳһ��¼��ʽ��
*   http://it.jeita.or.jp/document/publica/standard/exif/english/jeida49e.htm#a
* DCIM(Digital Camera In Memory ?)--����������ڴ洢��Ƭ���ļ���
* DDM(Data Disc Maker) -- ���ݹ���
* Deinterlace(������)-- ����ͳģ����Ӹ���625����ߵ�720P�ĸ����ؼ���,ȥ���˶�ͼ�����������ͼ���Եģ������,
*   �ɱ����ϸ��������,ʹ���������θк���ʵ�С�
* DLNA(Digital Living Network Alliance) --
* DRM(Digital Rights Management) -- ����Ȩ�ޱ���
* DSC(Digital Still Camera)--�����������
* DTS(Decode Time Stamp) -- ����ʱ�������ʶ�����ڴ��е�Bit����ʲôʱ��ʼ����������н��н��룬�ڽ���׶�ʹ�ã���GOP��B-֡��Ӱ��,
*   ������DTS��PTS��ͬ��ĳЩ��Ƶ�����ʽ��������˫��Ԥ�⼼��������B֡ʱ��DTS��PTS��һ��
* DV(Digital Video) -- �����������ʹ�� DV-AVI 2.0 + LPCM ����ʽ���棿
* DVD(Digital Video Disc) -- ������Ƶ���̡�����650���ײ����ĺ���д�������÷ֱ���Ϊ��720x480(NTSC��ʽ)��720x576(PAL��ʽ)�ȡ�
*   DVD-5 -- ����4.7GB�ĵ��浥��DVDƬ��DVD-9 -- ����Ϊ8.5GB�ĵ���˫��DVDƬ��
*   ? ����ʱ�� Video/Audio/SubPicture�����֡������CC��Line21Dec������Ƶ��DolbyDigital��LinerPCM����
* DVR -- ���������
* DVR-MS(*.dvr-ms) -- �� Windows XP Service Pack 1 ��������������棨Stream Buffer Engine��SBE��������ý���ļ���Media Center 
*   �����洢¼�Ƶĵ��ӽ�Ŀ��
* Exif(Exchangeable Image File Format)--�ɽ���ͼ���ļ���ʽ����ר��Ϊ�����������Ƭ�趨�ģ����Լ�¼
*   ������Ƭ��������Ϣ���������ݡ�http://www.exif.org
* Filmroll -- ������������ͼչ����
* FOURCC(four-character code) -- ���ֽ��룬���ڱ�����ý����������ͣ��� "RIFF","AVI ","LIST" ��,ͨ���� FCC('RIFF')�����塣
*   FOURCCMap fccMap = FCC('LIST'); pMediaType->subtype = static_cast<GUID>(fccMap); �� mmioFOURCC ?
* GOP(Group Of Pictures) -- �ɹ̶�ģʽ��һϵ��I֡��P֡��B֡��ɡ�����I֮֡���γ�һ��GOP��
*   I-֡(���ؼ�֡)����֡�ڱ��룬ͨ����ÿ��GOP�ĵ�һ��֡�������ʶȵ�ѹ������Ϊ������ʵĲο��㣬���Կ�����ѹ�����ͼ�񣬿���ֱ�ӽ�ѹ�ɵ���������ͼƬ��
*   P-֡����Ԥ����룬����ͼ��������ǰ���ѱ���֡��ʱ��������Ϣ��ѹ�����䣬��Ҫ�ο�ǰһ��I-֡��B-֡������������ͼƬ
*   B-֡����˫��Ԥ���ڲ���룬�ȿ�����Դͼ������ǰ���ѱ���֡��Ҳ�˼�Դͼ�����к����ѱ���֮֡���ʱ��������Ϣ��
*     ѹ�������������ı���ͼ��
*   I֡������Ϊ�˼��ٿռ������࣬P֡��B֡��Ϊ�˼���ʱ�������ࡣ
*   ��û��B֡���ڵ������DTS��˳���PTS��˳��Ӧ����һ���ġ����ڴ���˫��Ԥ��֡�������ͨ��Ҫ���������ͼ���������Ա�֤�����ͼ��˳��Ϊ��ʾ˳��
* Handycam(Handy Camera) -- Sony�����������Ʒ�ƣ��������䡢���ɡ�����ʹ�ú�Я�����ص㡣�洢���ʰ����Ŵ�(Type)��
*   DVD���̡�Ӳ��(HDD)������(Memore Stick)�ȣ������Ѿ�֧�ָ���(SDʹ��M2PS��ʽ��HDʹ��AVCHD��ʽ[m2ts?]����ֹ��ʹ��DCF)��
* HD DVD(High Definition DVD) -- ��֥�����ĸ����׼���Ѿ�ʧ�ܸ�Blu-ray��
* HDMV(high definition movie mode) -- 
* HDV -- �����ڴ�ͳ Mini DV �Ŵ��ϼ�¼ 1080i(AVC) �ĸ�����Ƶ����ͬʱ���ݱ��� DV ����������ʹ�� MPEG2-TS + MPEG-1 Audio Layer II ���档
* HDTV  -- High Definition TV,������ӣ���������ѹ���������԰�HDTV�ĵ���㲥��ѹ�������е�ģ�����Ƶ����Χ��
*   (6MHz��8MHz)���������͵ĵ��ӡ�
* IPP(Intel Performance Primitive) -- ��Ӧ��˲��ҹ��ܷḻ��һ��Library������һЩ�����ý�塢���ݴ���
*   ͨ��Ӧ�ó���ģ����ұ��Ż��ĺ�����
* ISO(International Standards Organization) -- ���ʱ�׼����֯��
* ISO 9660--��ǰΨһͨ�õĹ����ļ�ϵͳ���κ����͵ļ������֧���������е���¼���Ҳ��֧������
* ITU(International Telecommunications Union)-- ���ʵ������ˡ�
* ITU-T(TforITU Telecommunication Standardization Sector) -- ���ʵ�������Զ��ͨ�ű�׼���顣���ʵ������˹����µ�
*   ר���ƶ�Զ��ͨ����ع��ʱ�׼����֯��
* JVT()--������Ƶ�顣��ISO/IEC �� ITU-T������ɣ��ƶ���Ƶѹ�������׼��
* LAME -- Ŀǰ��õ�MP3��������,��ѣ���������VBR��ABR��CBR���ֱ���ģʽ.
*   ע�⣺Ĭ�������lame����ÿ����������ݲ��Ǹպ�һ֡�������������ÿһ�����ݰ�����Ϊһ֡���н��������޷�������
*         ������дAVIʱ��Lame���ݽ��л��棬����ÿһ֡��д��AVI��
* MasterContent -- ���� BD Authorʱ��Դ�ز�(PMB �� Set��BDMV�ļ�����)����Ҫ����UI��ʾ�ȡ���Ӧ��ConvertedContent��
* Mode -- ��ƵԴ��ģʽ����ΪVideMode��FilmMode��
*   Video Mode ͨ��ָ�����������ģ�NTSC(29.97fps) �� PAL(25fps)��
*   Film Mode �ǵ�Ӱģʽ����ÿ��24֡(fps)����
* Modd(MOvie aDditional Data) -- ���涯���ļ��ĸ�����Ϣ�ĸ����ļ���ʹ��SfCommonLib::SfModdXML����ж�д��
* Moff(MOvie Filmroll File) -- ��������ͼ�ļ�����MOFF�ļ���ȡ������ͼ��ֱ�ӴӶ����ļ���ȡ��Ҫ�죬�����Ч�ʡ�
* Motion JPEG -- �����AVI����������AVI��һ��
* MPEG(Motion Picture Experts Group) 
* MPLS(Movie PlayLists) -- (AVCHD �ϵ�һ���ļ��� AVIndex �����һ��Content)
* MultiLUN(Multiple Logical Unit Number)--�߼���Ԫ�ţ�LUN��ָ����һ������SCSI���ߵ�Ψһ��ʶ��ţ�������ʹ��
*   �����������˸��豸������ÿ������һ���߼���Ԫ����ÿ��LUN��һ���ܹ�ʶ���ض��߼���Ԫ��Ψһʶ��ţ�
*   ��������һ�������û���һ���ļ�����һ��������һ��USBӲ�̿��ܻ���N��������
* NTSC(National Television System Committee)--ȫ��������ʽίԱ�ᡣ���������ҵ��ӱ�׼ίԱ�ᣨNTSC���ƶ���
*   ��ɫ���ӹ㲥��׼��������Ҫ��֧��NTSC-J��NTSC-US���������ձ��ȵ�ʹ�á�
*   ����ͬʱ�ƣ�֡ƵΪÿ��29.97fps��525��ɨ���ߣ�����ɨ�裬�������Ϊ4��3���ֱ���Ϊ720x480��
* OGG -- һ���Ӵ�Ķ�ý�忪���ƻ�����Ŀ���ƣ� ���漰��Ƶ��Ƶ�ȷ���ı��뿪�����ṩ��ȫ��Ѷ�ý����뷽��
* PAL(Phase Alternating Line) -- ���е��࣬���ӹ㲥��ɫ�ʵ�Ƶ��һ�ַ�����625��ɨ���ߣ�25fps������ɨ�飬PALɫ�ʵ�Ƶ��
*   ����ԭ�кڰ׵��ӹ㲥��ʽ������¼����ɫѶ�š��й���Ӣ������۵ȵ�ʹ��(ŷ�ޱ�׼)��
* PanoramaJPEG -- 360��ȫ����Ӱ(�ѥΥ�޴�Ӱ)������JPEG
* PCM(Pulse Code Modulation) --  ���������ƣ����ʺõ������
* PlayListMark -- (AVCHD�е��ֶ��ָ�㣿)
* PMB(Picture Motion Brower) -- Ҳ��ΪFoothold��PictureGear Studio
* PND(Personal Navigation Device) -- 
* Progressive() -- ?
* Progressive JPEG -- һ�ָ߼�JPEG��ʽ����װ��ͼ��ʱ����ʼֻ��ʾһ��ģ����ͼ���������ݵ�װ�룬ͼ���𲽱��������
* PSNR -- ��ֵ�����,��ʾ�ź������ܹ��ʺ�Ӱ�����ı�ʾ���ȵ��ƻ����������ʵı�ֵ�Ĺ���������ö����ֱ���λ����ʾ��
* PTP(Picture Transfer Protocol) -- ͼƬ����Э�飬�ɿ´﹫˾��΢��Э���ƶ���һ�ֱ�׼���������ֱ�׼��ͼ���豸
*   �ڽ���Windows XPϵͳ֮����Ը��õر�ϵͳ��Ӧ�ó���������ϵͳ����ֱ�ӷ�����Щ�豸���ڽ����������ʱͼ
*   Ƭ���ϴ�����������ʱͼƬ�Ĵ��͵ȡ�
*   �Զ����ʱ�����ĶԻ���(��Ҫ����GUID?)��HKLM\SYSTEM\ControlSet001\Control\StillImage\Events\Connected\{00F3CDFD-5D2E-439F-8900-3F56A0C1C8BA}
* PTS(Presentation Time Stamp) -- ��ʾʱ��������ڶ�����������Ƶ֡ʲôʱ����ʾ������������Ƶ��ͬ�����������ͨ��������ʾ˳�򣿣�
* PureVideo -- Nvidia �������Կ�ӰƬ�����Լ����ټ��㼼����GPU�ڽ��Ŀɱ����Ƶ����������Զ�MEPG-2��HD Video��Ƶ���Ž���Ӳ�����٣�
*   ��ɼ�ͥӰԺ�����720p��1080i��Ƶ���ʡ���Ҫ��װnVIDIA DVD Decoder��֧�ָ�ʽΪMEPG2��WMV��H.264��VC-1�ȡ�
*   ��������������֣�ͼ�δ���������(���) �� PureVideo DVD������(�շ�)
* Re-Mux -- ��(MPEG-2 TS���ļ���һ���ִ���Audio��Videoû��ͬ�����������Ҫ����Re-muxȡ��ͬ��)
* RIFF(Resource Interchange File Format) -- ʹ�����ַ���FOURCC(С�˱�ʾ?)�������������͡�
*   �ļ���ʽΪ: "RIFF" + ���ֽ����ݴ�С(ʵ�����ݳ��ȼ�4) + FOURCC������(�� "AVI ") + ʵ�����ݣ�
*     ע�⣺���ݴ�С������RIFF�����ݴ�С���򣬵����ļ���С-8
*     List(�б�) -- �б����Ƕ�����б�Ϳ顣��ʽΪ��
*       "LIST" + ���ֽڵ�listSize(listData���ȼ�4) + listType(�� hdrl, strl��) + listData
*     Chunk(��) -- ��ʽΪ��
*       ���ֽڵ�ckID + ���ֽڵ�ckSize(ckData�ĳ���) + ckData
*
* RLE(Run-Length Encoded) -- һ��BMP��ѹ����ʽ��
* RODD(ROw aDditional Data) -- ����RAW�ļ��ĸ�����Ϣ�ĸ����ļ�����ʽͬMODD��ȫһ����
* ROM Mark -- ��һ������������ϣ�������������������֮�У������ؼ���ֹ�������������ܵ�δ�õ���Ȩ��
*   ���ų�ʽ���н������ݡ�
* SBR(Spectral Band Replication) -- Ƶ�θ���,һ���µ���Ƶ������ǿ�㷨����mp3PRO��ʹ�ã�������ָ����λ����������Ƶ�Ĵ������Ʊ���Ч�ʣ�
*   �� ���������㷨������ֱ��Ӱ�쵽���ʵĺû�(SBR��������ݸ�����һ�ֲ�����Ƶ�����)
* ScrapBook -- Sony����еļ��������ܣ��ɽ��������Ƭ�Զ���������ᡣ����󣬿ɸ����������ں�Ƶ������Ļ���Զ����ϱ������档
*   �ᴥ����������ʾ��ĳ��С�ߴ���Ƭ�����ɻطŸ���Ƭ������ṩ16�ֱ�����������ѡ����������װ��������Ƭ����ʹ�����������
* SD -- ��׼����
* SDTV -- ���ֱ�׼�������ȵ��ӣ�����MPEG-2ѹ��(ͬDVDһ��)���������Ǻ����ߵ��ӵ���㲥��
* Seamless -- ��SD �;�ֹ��ʱ��Ч��������ʲô��˼��)
* SOI(Start Of Image)--SOI��Exif�б�����Ϊһ��Marker Segments��ÿ��Marker�ĵ�λ��2���ֽڣ���һ��word����
*   ����ֵ��SOI Marker���ļ���ʼ��λ�á�JPEG��ʽ�ļ���SOIֵΪFFD8��ͨ��SOI!=FFD8��JPEG�ļ��ǲ���֧�ֵĸ�ʽ��
* SPU(Sony Picture Utility) -- �ṩ Sony Digital Camera & HandyCam ý������ṩͳһ������
* STI(Still Image Architecture)--?
* Time-Shifting(ʱ��ƽ��) -- ��ͬ������¼���ŵĹ��ܣ���������ʱ�뿪���ӣ�ֻ��ʹ��"Time-Shifting"���ܼ�����¼�ƣ�Ȼ�������������
* Transfer JET -- Sony��ר�õģ���һ�ַǽӴ�ʱ���䷽ʽ��JET�豸����PC����DSC/CAM����JET�豸�ϣ�����wire���ӣ���
*  �Ϳ��Ը�PC�˽��д��䡣Ŀǰ����Importerʱ��֧�֣�Exproterʱ���������ɣ����ȶ������׶Ͽ���
* UDF(Universal Disc Format)--���ñ�׼�ķ�װд�뼼����PW��Packet Writing����CD-R/CD-RW����Ӳ��ʹ�ã�
*   �û������ڹ������޸ĺ�ɾ���ļ�������UDF��ʽ���п�¼ʱ����¼��������ݴ���������ڴ�����ʱ����
*   һ�������ļ�Ŀ¼��ͬʱ�ӹ�ϵͳ�Թ��̵ķ��ʡ�
* VBR(Variable Bit Rate)-- ��̬���ʣ�ѹ�������ѹ��ʱ������Ƶ���ݼ�ʱȷ��ʹ��ʲô�����ʣ�����������Ϊǰ�����ļ���С�ķ�ʽ��
*   ���Ƽ��ı���ģʽ��ͨ���ܹ������������֮��ȡ�þ��ѵ�ƽ�⡣
* VCD -- ���ּ�������
* VCEG() -- ��Ƶ����ר���飬����ITU-T
* VBI(Vertical Blanking Interval) -- ��Ƶ�ɼ���ʹ�õ�
* VFW(Video for Windows) -- ΢��92���Ƴ��Ĺ���������Ƶ��һ��  VFW logo�����������VFW�ĺ�����AVI�ļ���׼��
*    Χ��AVI�ļ���VFW�Ƴ���һ������������Ƶ�ɼ���ѹ������ѹ�����طźͱ༭��API��
* VHS(Video Home System) -- ����¼��ϵͳ��
* VOB(Video OBjects) -- ��ƵĿ���ļ�,DVD����Ƶ�ļ�����һ����Ƶ(MPEG2)���������(AC3/LPCM��)����Ļ��������ɡ�
*   һ��DVD��������9�ְ�����32����Ļ��
* VOD(Video-On-Demand) -- ��Ƶ�㲥���û�ͨ�������ŵ��㲥��Ҫ���ý�Ŀ�����������ݿ����������Ľ�Ŀ����ѹ��
*   ���������ŵ������û�����Ҫ������VOD�ͻ���ϵͳ��
*   TVOD(True VOD)--���㼴����
*   NVOD(Near VOD)--׼ʵʱ�㲥��
*   IVOD(Interactive VOD)--����ʽ�㲥
* WDM(Windows Driver Model)--
* WIA(Windows Image Acquisition)--Windows ͼ��ɼ���WinME��WinXP��ϵͳ���롣�ṩ�㷺��֧��������Imaging�豸
*   (��ɨ���Ǻ��������)����ͼ��Ӧ�ó���(��PhotoShop)�Խӡ�ʹ��WIA��������ֻ��Ҫд WIA microdriver(ʹ��WIA DDI��)
*   �������豸ʹ�ã�������WIA����ɲ���ϵͳʵ�֡�WIA ��ʹ�ý��������ʵ�ֵ�COM���,�ṩ�Լ������ݴ��ͻ���
*   (IWiaDataTransfer�ӿڲ�ʹ�ù����ڴ�)��������ͼ�����ݴ��͹����е�������ʧ��
*   �����ڣ�1.ö�ٿ��õ�ͼ���ȡ�豸;2.ͬʱ���Ӷ���豸;3.��ѯ�豸���ԣ���ȡ����;4.��ȡ�������豸�¼�֪ͨ��Ϣ �ȡ�
*   http://msdn.microsoft.com/en-us/library/dd393559.aspx
* WMA(Windows Media Authoring) -- ΢���Ʒ�ģ���ת��ΪWMVʱ����֤(��WMVCore.dll�ṩ����Dll���ṩ��Playback����)
* WMT(Windows Media Technology)--
* WST(World Standard Teletext)
* x.v.Colourɫ���׼ -- ������������xvYCC����ɫ���׼��һ������ñ�׼�漰��ɫ�ʷ�Χ��Ŀǰʹ�õĹ㲥���ӱ�׼��sRGB������
* xdb()--(Sony)ʵ������Ϣ�������ݿ⣬���б�����Meta data��albums��favorite����Ϣ��
*******************************************************************************************************/

Windows Media Player7 SDK
Windows Media Format 7.1.1 SDK 
Windows Media Encoder 7.1 SDK 
Windows Media Rights Manager 7.1 SDK 
Windows Media Services SDK 


#endif //MULTI_MEDIA_GLOSSARY_H