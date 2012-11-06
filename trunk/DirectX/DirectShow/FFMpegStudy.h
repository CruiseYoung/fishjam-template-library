#pragma once

//CPP �а���ffmpegͷ�ļ�ʱ����Ҫʹ�� extern "C" ����ΪC�������ӷ�ʽ
#if 0
#  ifdef __cplusplus
extern "C" {
#  endif

#include <libavcodec\avcodec.h>  
#include <libavformat\avformat.h>  
#include <libswscale\swscale.h>  

#  ifdef __cplusplus
}
#  endif

#endif

//http://blog.sina.com.cn/s/blog_51396f890100o1yf.html
//http://www.cnblogs.com/billcoco/archive/2012/06/18/2553939.html -- FFMpegдMP4�ļ����ӷ���
//http://hi.baidu.com/y11022053/item/6d4c34ba87c7b5f362388e9a  ffmpeg����h264
//��Ƶ������վ: fms(����¼�ơ�����)+ffmpeg(��̨��ʽת��)

/* -- �� h264 �ı��뷽ʽ
$ ./configure --enable-shared --enable-w32threads --disable-static --enable-memalign-hack 
--disable-yasm --enable-libx264 --enable-gpl --extra-ldflags="-L/usr/local/lib -W1,-add-stdcall-alies" --extra-cflags="-I/usr/local/include" --enable-swscale
*/

/*********************************************************************************************
* �������ڴ����
*   1.av_image_alloc -- �Ƽ���ʽ?
*   2.avpicture_get_size ��ȡ��С�� av_malloc

* AVDictionary -- һ����ֵ�Ե����飬ͨ���ڻ�ȡ�����ò���ʱʹ��
* 
* AVFrame
*   data/linesize -- FFMpeg�ڲ���ƽ��ķ�ʽ�洢ԭʼͼ�����ݣ�����ͼ�����ط�Ϊ���ƽ�棨R/G/B��Y/U/V����
*      data�����ڵ�ָ��ָ���������ƽ�����ʼλ�ã�linesize�������Ÿ�����������ƽ��Ļ��������п�
*   key_frame -- ��ͼ���Ƿ��ǹؼ�֡
*   pict_type -- ��ͼ��ı������ͣ�I(1)/P(2)/B(3)
*		
* AVStream -- ����һ��ý��������󲿷���Ϣ��ͨ�� avformat_open_input �����ļ�ͷ��Ϣȷ����������Ϣ��ͨ�� avformat_find_stream_info ��ȡ
*   ���͵��� ��Ƶ������Ӣ����Ƶ������Ӣ����Ļ��(Subtitle)����ͨ�� av_new_stream��avformat_new_stream �ȴ���
*   index -- ��AVFormatContext��������������ֵ�Զ�����(AVFormatContext::streams[index])
*   nb_frames -- ���ڵ�֡��Ŀ
*   r_frame_rate -- 
*   time_base -- ����ʱ���׼����һ��ʵ����������ý�����ݵ�pts��dts���������ʱ���׼Ϊ���ȡ�ͨ����ʹ��av_rescale/av_rescale_q����ʵ�ֲ�ͬʱ���׼��ת��
* AVFormatContext -- ������һ��ý���ļ���ý�����Ĺ��ɺͻ�����Ϣ���� avformat_open_input ��������ʼ������ֵ��
*   ������һЩֵ(�� mux_rate��key ��)��Ҫ�ֹ����ó�ʼֵ��������ܳ����쳣��
*   debug -- ���Ա�־
*   nb_streams/streams -- AVStream�ṹָ������, ������������Ƕý����������
* AVPacket -- �ݴ����֮ǰ��ý�����ݣ�һ����/��Ƶ֡��һ����Ļ���ȣ���������Ϣ������ʱ�������ʾʱ�����ʱ����),
*   ��Ҫ���ڽ�����������װ�����ݣ�
*   data/size/pos -- ���ݻ�����ָ�롢���Ⱥ�ý�����е��ֽ�ƫ����
*   flags -- ��־�����ϣ�1(AV_PKT_FLAG_KEY)��ʾ��������һ���ؼ�֡, 2(AV_PKT_FLAG_CORRUPT)��ʾ�������Ѿ���
*   destruct -- �ͷ����ݻ������ĺ���ָ�룬��ֵ��Ϊ [av_destruct_packet]/av_destruct_packet_nofree, �ᱻ av_free_packet ����
*
* avformat_find_stream_info -- ��ȡ��Ҫ�ı����������(�� AVMediaType��CodecID )�����õ� AVFormatContext::streams[i]::codec ��
* av_read_frame -- �Ӷ�ý���ļ����ý�����ж�ȡý�����ݣ���ȡ�������� AVPacket �����
* av_seek_frame -- �ı�ý���ļ��Ķ�дָ����ʵ�ֶ�ý���ļ���������ʣ�ͨ��֧�ֻ���ʱ�䡢�ļ�ƫ�ơ�֡��(AVSEEK_FLAG_FRAME)��������ʷ�ʽ��
* AVCodec
*   avcodec_find_decoder/avcodec_find_encoder -- ���ݸ�����codec id����������ƴ�ϵͳ����Ѱ������һ��AVCodec�ṹ��ָ��
*   avcodec_alloc_context -- 
*   avcodec_open/avcodec_open2/avcodec_close -- ���ݸ����� AVCodec �򿪶�Ӧ��Codec������ʼ�� AVCodecContext/ �ر�Codec��
*   avcodec_alloc_frame -- ��������ʱ�� AVFrame �ṹ
*   avcodec_decode_video2 -- ����һ����Ƶ֡������������AVPacket�ṹ�У����������AVFrame�ṹ�С�
*   avcodec_decode_audio4 -- ����һ����Ƶ֡������������AVPacket�ṹ�У����������AVFrame�ṹ��
*   
* ͼ�����ź͸�ʽת��(sws_scale�������� img_convert)���� RGB24 <-> YUV420P
*   ת���㷨�ĶԱȣ�http://www.cnblogs.com/acloud/archive/2011/10/29/sws_scale.html
*     SWS_FAST_BILINEAR Ч��Ч���ƺ�������(Ĭ���Ƽ�?); SWS_POINT ��СʱЧ�ʺܸߣ���ͼ��Ч���Բ
*   sws_getContext -- �������롢���ɫ�ʿռ�Ĳ����õ�ת��������
*   sws_scale -- ת��
*   
* ʱ��
*   ���ñ����Packet��pts: pkt.pts = av_rescale_q(c->coded_frame->pts, c->time_base, video_st->time_base); 
*
* �ļ�����
*   av_open_input_file -> av_find_stream_info(��ȡ����Ƶ����Ϣ) -> 
*   avformat_write_header -> av_write_frame -> av_write_trailer
*   av_read_frame / av_write_frame
*********************************************************************************************/

/**********************************************************************************************
* MainConcept -- ��ҵ�ı����⣬Camtasia ����ʹ��) http://www.mainconcept.com, �ƺ��� DivX ��������?
*   TechSmith Screen Capture Codec(C:\Windows\System32\tsccvid.dll)
* 
* ffdshow -- һ����ѵı�����������װ����DirectShow�ı�׼�����
*   ʹ�� libavcodec library �Լ��������ֿ���Դ����������(�� DivX��)����֧��H.264��FLV��MPEG-4�ȸ�ʽ��Ƶ
*  
* ffmpeg -- ��Դ�ҿ�ƽ̨������Ƶ�����������һ�ױ����Ŀ�ܣ����вɼ������롢�����ȹ���
*   http://www.ffmpeg.org/ <== ��ʽ���� (git clone git://source.ffmpeg.org/ffmpeg.git ffmpeg)
*   http://www.ffmpeg.com.cn <== ������վ���ṩ����õ� FFmpeg SDK���أ��� FFmpeg SDK �����ֲ�
*   http://ffmpeg.zeranoe.com/builds/
*   http://bbs.chinavideo.org/viewthread.php?tid=1897&extra=page=1 <== Windows �°�װ�ͱ���
*   http://dranger.com/ffmpeg <== Ӣ�ĵ����Ž̲ģ�����Щ��ʱ
* ģ��
*   ffmpeg -i �����ļ� ����ļ� -- ��Ƶ�ļ�ת�������й���,Ҳ֧�־���ʵʱ���ӿ�ץȡ�ͱ������Ƶ�ļ�
*     ffmpeg.exe -h ��ʾ��ϸ����
*     ���ӣ� ffmpeg.exe -i snatch_1.vob -f avi -vcodec mpeg4 -acodec mp3 snatch.avi
*     -acodec codec		<== ָ����Ƶ���룬�� aac 
*     -an				<== ��ֹaudio
*     -aspect 16:9		<== ���ó���ȣ��� 4:3 �� 16:9 ��
*     -b <������>       <== ������Ƶ�ı����ʣ���Ϊ -b:a(�Զ�?) �� -b:v ����
*     -bf <int>			<== ʹ��B֡
*     -f format			<== ָ���ļ���ʽ, �� avi/mp4 ��(���ø�ʽ��ͨ�� -formats �鿴)
*     -g groupSize		<== ����GOP��С�������ó� 300 ��ζ��29.97֡Ƶ��ÿ10�����INTRA֡
*     -qblur <float>	<== ָ����Ƶ������ģ��0. ~ 1.0(ԽС����Խ��?)
*     -qscale <float>   <== 0.01~255 ?
*     -s WxH			<== ָ����С(��x��)
*     -strict <int>     <== ָ����codec�ȵ�ʹ�����ƣ�ʵ���Ե�codec(��mpeg4)Ĭ���ǲ����õ�(Codec is experimental)����Ҫʹ�� -2 ������
*     -t ����ʱ��		<== ָ��ʱ�䳤�ȣ��� 20.000 ��ʾ 20s
*     -threads <int>    <== ָ���̸߳�����Ĭ��Ϊ auto ?
*     -vcodec codec		<== ָ����Ƶ���룬�� mpeg4/h264/libxvid ��(���ñ����ͨ�� -codecs �鿴)
*     -target type		<== ָ���ļ����ͣ��� vcd/dvd �ȣ��ܴ�̶��Ͽ��Ծ�������
*     -y                <== ֱ�Ӹ�������ļ���������ʾ
*   ffserver -- ����HTTP(RTSP���ڿ�����)����ʵʱ�㲥�Ķ�ý�������.Ҳ֧��ʱ��ƽ��(Time-Shifting)
*   ffplay -- �� SDL��FFmpeg�⿪����һ���򵥵�ý�岥����(��Ҫ�Ȱ�װ SDL ����ܱ���)
*   libavcodec -- ����������FFmpeg����Ƶ��������Ŀ�.Ϊ�˱�֤�������ܺ͸߿ɸ�����,��������������ͷ������
*   libavformat -- ���������е���ͨ����Ƶ��ʽ�Ľ������Ͳ������Ŀ�
*   libavutil -- �����⣬ʵ����CRCУ����Ĳ�����128λ������ѧ�����Լ������������������ȡ�������ڴ���䣬���С�˸�ʽ��ת���ȹ��ܡ�
*   libswscale -- ��ʽת���Ŀ⣬������Ҫ�� RGB ת��Ϊ YUV
*   libpostproc -- 
* 
* ����͵���(�޷�ʹ��VC���룿)��������MSys+MinGW���룬���Ǳ��������DLL�ǿ��Ա�VCʹ�õ�
*   http://blog.csdn.net/jszj/article/details/4028716  -- Windows �±���
*   http://www.cnblogs.com/mcodec/articles/1659671.html -- ʹ��VC����ķ���
*   http://ffmpeg.org/trac/ffmpeg/wiki/How%20to%20setup%20Eclipse%20IDE%20for%20FFmpeg%20development -- ����eclipse�Ļ���
*     1.�� http://www.eclipse.org/downloads/ ���� "Eclipse Classic"����ֱ�����ش� C/C++ �İ汾
*     2.���߰�װCDT���: Help -> Install New Software -> http://download.eclipse.org/releases/indigo -> ����CDT
*     3.���߰�װGit���(http://www.vogella.de/articles/EGit/article.html), 
*     4.��װMinGW
*     4.Clone FFMpeg ��Git�ֿ�: File->Import->Git->Projects From Git(git://source.ffmpeg.org/ffmpeg.git)
*     5.��������: File->New->Project-> C/C++ -> Makefile Project with Existing Code 
* 
*   http://ffmpeg.zeranoe.com/blog/  -- �Զ����غͱ���ffmpeg�Ľű�
*   ����ѡ�� -- ע�⣺���밲װ��������ʱ���Ƽ�ʹ�� --prefix=/usr(Ĭ���� /usr/local)������ffmpeg�����Զ��ҵ���صĿ�(������Ҫ�ֹ�ָ��)
*     ./configure --enable-w32threads --enable-shared --enable-static --enable-memalign-hack --enable-libx264 
*        --enable-libxvid --enable-gpl --extra-cflags=-I/usr/local/include --extra-ldflags=-L/usr/local/lib
*        && make && make install
*     --prefix=PREFIX           ���ð�װ·��
*     --disable-debug			���õ�����Ϣ,���뷢���汾
*     --disable-mmx             
*     --disable-optimizations	��ֹ�Ż���һ�����ڵ��԰汾
*     --disable-static			���þ�̬��
*     --disable-stripping       һ�����ڵ��԰汾
*     --disable-yasm            ��ֹʹ��yasm��������԰�װ yasm��ʹ�ã�(��������?)
*     --enable-amr_nb/--enable-amr_wb ֧���ֻ���3gp��ʽ(www.3gpp.org/ftp/Specs/archive/26_series/26.204/26204-510.zip����ѹ����ļ���Ҫ������ libavcodec/amrwb_float )
*     --enable-avisynth			���� avisynth(��Ƶ�ļ����ڴ�����)
*     --enable-faad				֧�� faad2��faac(www.audiocoding.com), ./configure --prefix=/usr --with-mp4v2 --enable-shared
*     --enable-gpl				��ѭgplЭ��,��ʹ�� x264/xvid ����ѭgplЭ��Ŀ�ʱ����ָ��
*     --enable-libfaac          ֧��aac(������ѵ�),��Ҫ�ȵ� faac Ŀ¼�±���, ./bootstrap<CR> ./configure xxx<CR> make
*     --enable-libx264			ʹ��x264��Ϊh264������,��ʾҪʹ�õ�3����x264,��ʱmingw include �� lib�ڱ����Ѿ���x264.h��libx264.a
*                               ftp://ftp.videolan.org/pub/x264/snapshots/ ����(���� git clone git://git.videolan.org/x264.git)
*                               Ȼ�� ./configure --prefix=/usr --enable-shared --enable-static --enable-win32thread && make && make install
*                               ע�⣺������ffmpegʱ��Ҫͨ�� -I �� -L ָ������Ŀ¼��������ܻᱨ��"libx264 not found"�����⡣
*     --enable-libxvid			���� Xvid ������(www.xvid.org), MPEG-4 video�ı���뷽��
*                               �� build/win32 Ŀ¼����VC�Ĺ���, MingGw �±����� "process_begin: CreateProcess(NULL" �Ĵ���,ԭ�� ?
*     --enable-libmp3lame		ʹ��mp3lame��Ϊmp3����������Ҫ�ȵ� lame Ŀ¼�±��루sourceforge.net/projects/lame/files/��
*                               ./configure --enable-shared --prefix=/usr 
*     --enable-memalign-hack    �����ڴ���룬Windows�±���
*     --enable-shared			���붯̬��
*     --enable-static           ���뾲̬��
*     --enable-small			�����ļ���С����
*     --enable-w32threads       ������߳�֧�֣�������������Ч��
*     --extra-cflags=-O0        ����ı���������� -O0 ��ʾ���Ż��� -I/usr/local/include ��ʾͷ�ļ�Ŀ¼
*     --extra-ldflags=xxx       ��������Ӳ������� -L/usr/local/lib ��ʾ����Ŀ¼
*     --enable-pthreads         linux�������߳�?
*  ����
*    av_log_set_callback -- ������־�Ļص�����
*    av_log_set_level -- ������־�ȼ�(�� AV_LOG_DEBUG )
*    av_dump_format -- ���� FormatCtx ����Ϣ
* 
*  �����⣨ffmpeg ֻ��һ����ܣ���ͬ�ı������ڲ�ͬ����վ��http://ffmpeg.zeranoe.com/builds/����
*    ��ͨ�� --extra-cflags ��ָ��ͷ�ļ����ļ���?
*    libavfilter -- ���libswscale��������ͼ���pixel formatת��������DirectShow ���ƣ�
*      AVFilterGraph -- ������ȫ��ͬ��directShow�е�fitlerGraph
*      AVFilter -- ����һ��Filter
*      AVFilterPad -- ����һ��filter�����������ڣ���ͬ��DShow�е�Pin
*      AVFilterLink -- �����������ӵ�fitler֮���ճ����
*    libx264 
*    libfaac 
*    libmp3lame -- linux ��֧��mp3
*    libogg 
*    libspeex 
*    libswscale -- 
*    libvorbis 
*    libtheora  
* 
*
*********************************************************************************************/

/*********************************************************************************************
*  ��������
*    ��������: video.avi(Container) -> �򿪵õ� Video_stream -> ��ȡPacket -> ������ Frame -> ��ʾFrame
*    Container -- ������Ƶ�е�������һ��ָ����һ���ض����ļ���ʽ(�� AVI/QT )������ָ����������������Ƶ����Ļ�������Ϣ
*    Stream -- ý������ָʱ�����ϵ�һ���������ݣ���һ����������Ƶ����Ļ����
*    Frame -- Stream�е�һ�����ݵ�Ԫ(AVFrame?)
*    Packet -- Stream�е�Raw����,�����˿��Ա�����ɷ������������Ӧ�ó����в�����֡��ԭʼ����(ע��Packet��Frame��һ�����)
*    CODEC -- �������(Code �� Decode)���� Divx�� MP3,��֡Ϊ��λʵ��ѹ�����ݺ�ԭʼ����֮����໥ת��
*     
*
*********************************************************************************************/

/*********************************************************************************************
* ����˵��
*   av_register_all     -- ע������������ʽ(Format)��CODEC
*   av_open_input_file  -- ���ļ���avformat_close_input_file �ر�
*   av_find_stream_info -- ���ļ�����ȡ����Ϣ
*     avformat_find_stream_info ?
*   avcodec_find_decoder-- ���� decoder /encoder?
*   avcodec_open        -- �򿪱��������ʹ����Ϻ���Ҫͨ�� avcodec_close �ر��ͷ�
*   avcodec_alloc_frame -- Ϊ����֡�����ڴ�
*   av_read_frame       -- ����������ȡ��֡���ݣ�һ����Ҫ��ѭ���н���
*   avcodec_decode_video-- �ж�֡�����ͣ�������Ƶ֡����:


* 
*  Դ�����
*    AVPicture
*     +-AVFrame
*    AVPacket -- av_read_frame��ȡһ�������Ұ������浽AVPacket::dataָ��ָ����ڴ���(ffmpeg����?)
*    
*    AVFormatContext -- ͨ���� ->streams->codec->codec_type �ж����ͣ��� CODEC_TYPE_VIDEO
*      AVInputFormat
*      AVOutputFormat
*    AVCodecContext -- ������������ģ�������������ʹ�õĹ��ڱ��������������Ϣ��
*      bits_per_coded_sample -- 
*      codec_name[32]��codec_type��codec_id��codec_tag -- ������������֡����͡�ID��FOURCC ����Ϣ
*      sample_fmt -- ��Ƶ��ԭʼ������ʽ, �� SampleFormat ö��
*      time_base -- ���÷���(den/num)������֡�ʵ���Ϣ,�� 25fps(25/1) NTSC�� 29.97fps(den/num= (framenum / 17982) / (framenum % 17982)) ?
*    AVCodec -- ͨ�� avcodec_find_decoder ���ң� avcodec_open2 ��
*    SwsContext -- ?
* 
*    Demuxer -- ý���ʽ������
*    struct AVCodec -- codec �����ݽṹ��ȫ�ֱ��� *first_avcodec, avcodec_register_all ����ע��ȫ�� codec
*    ���벥�ţ�Media -> ByteIOContext -> Demuxer -> Decode -> YUV -> Display
*    ��ʽת����Media -> ByteIOContext -> Demuxer -> Decode -> YUV -> Encode -> Muxer -> ByteIOContex -> Media
*    ��ʽת������ �ļ����ʹ� .flv -> mp4
*    �������ͣ��� codec �� mpeg4 -> H264
*    ý��¼�ƣ�YUV -> Encode -> Muxer -> ByteIOContex -> Media
*
* ��������
*   av_register_all();  //ע�������п��õ��ļ���ʽ�ͱ�������Ŀ�(����ͨ�� XXXPolicy.xml �ļ���ָ��֧�ֵĸ�ʽ ?)
*   AVCodec *pCodecH264 = avcodec_find_encoder(CODEC_ID_H264); //����h264������
*   AVCodecContext *pContext = avcodec_alloc_context3(pCodecH264); //��ȡ��ʼ�ı����������ģ��������ñ������Ĳ���
*   pContext->codec_id = pCodecH264->id; //CODEC_ID_H264
*   pContext->codec_type = AVMEDIA_TYPE_VIDEO;
*   pContext->bit_rate = 3000000; pContext->width=nWidth; pContext->height=nHeight;
*   pContext->time_base.num = 1; pContext->time_base.den = 25; //ÿ��25֡, �� den=(int)(UNITS / pVih->AvgTimePerFrame);
*   pContext->gop_size = 12; //ÿ12֡��һ��GOP
*   pContext->thread_count = 2; //���̸߳���
*   pContext->thread_type = FF_THREAD_SLICE;  //����ʹ�ö���߳���ͬʱ�����һ֡
*   pContext->pix_fmt = PIX_FMT_YUV420P;  //������ɫ�ռ��ʽ?
*   //pContext->flags |= CODEC_FLAG_GLOBAL_HEADER; //mp4,mov,3gp �������������?
*   avcodec_open2(pContext, pCodecH264, NULL); //�򿪱���⣬ʹ����Ϻ���Ҫ avcodec_close
*   AVFrame *m_pRGBFrame = avcodec_alloc_frame();  //RGB֡����
*   numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height); //������Ҫ�Ŀռ�
*   rgb_buff = av_malloc(numBytes*sizeof(uint8_t));  //�����ڴ�, av_malloc ���Ա�֤�ڴ����
*   avpicture_fill((AVPicture*)m_pRGBFrame, (uint8_t*)rgb_buff, PIX_FMT_RGB24, nWidth, nHeight); //���ԭʼ��RGB����
*  
*   //ffmpeg�д��������Ƶ��ʽ����ֻ֧��YUV��ʽ����Ƶ֡(AVFrame)�������Ȱ�RGB����Ƶ֡ת��YUV��Ƶ֡(sws_scale),
*   //  m_pYUVFrame �� data[0]������,data[1]��ɫ��,data[2] Ҳ��ɫ��?
*   //  ���ǲ���ʱ����Ҫת��Ϊ RGB ��ʽ ?
*   SwsContext * scxt = sws_getContext(pContext->width, pContext->height, PIX_FMT_RGB24, 
*      pContext->width, pContext->height, PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);  //�ɵķ�ʽ�� img_convert ?
*   sws_scale ������?( ctx,pFrameRGB->data,pFrameRGB->linesize,0,pCodecCtx->height,pFrame->data,pFrame->linesize);  
*   int iOutSize = avcodec_encode_video(m_pContext, pBufferOut, lOutBufferSize, m_pYuvFrame);//����
*     //�� avcodec_encode_video2(c, &avpkt, m_pYUVFrame, &got_packet_ptr); 
*     //avcodec_decode_video/avcodec_decode_video2 �Ѱ�ת��Ϊ֡���õ�������һ֡ʱ����������֡������־frameFinished
*   sws_freeContext(scxt)
*   av_free_packet(&packet);
*    
*
* libavFilter ʹ������
*   AVFilterGraph *graph = avfilter_graph_alloc();
*   AVFilterContext *filt_src = NULL;
*   avfilter_graph_create_filter(&filt_src, &input_filter, "src",NULL, is, graph); //input_filter�����ɵ�����ʵ��
*   AVFilterContext *filt_out = NULL;
*   avfilter_graph_create_filter(&filt_out, avfilter_get_by_name("buffersink"), "out", NULL, pix_fmts, graph);
*   avfilter_link(filt_src, 0, filt_out, 0); //����source��sink
*   avfilter_graph_config(graph, NULL); //
*   ʵ��input_filter
*     static AVFilter input_filter = { .name="ffplay_input", .priv_size=sizeof(FilterPriv), .init=input_init, xxx};
*     
*   
*********************************************************************************************/

/*********************************************************************************************
* H264
*   Ӱ������Ч�ʵ�ά�ȣ�
*     1.��ͬ�ֱ���(Qcif/Cif)
*     2.��ͬ�����ӳ̶� -- ֡��ѹ��?
*     3.��ͬ�˶����ҳ̶� -- ֡��ѹ��?
*
*   ���ֿ�ԴH264����������(http://wenku.baidu.com/view/1d65a3717fd5360cba1adbc1.html)
*     1.JM encoder -- H.264�Ĺٷ�Դ��(У��ģ��)�����븴�Ӷȼ��ߣ�����ʵ��
*     2.T264 encoder -- �й���Ƶ����������֯���Ͽ�����264������������������������׼��264����(����������������?)
*     3.x264 encoder -- ����264��׼�����ı�������ע��ʵ�á��ڲ����Խ��ͱ������ܵ�ǰ���£����ͱ���ļ��㸴�Ӷ�
*           (������264��һЩ�Ա������ܹ���΢С�����㸴�Ӷȸߵ����� -- ���ο�֡��֤��Ԥ���в���Ҫ�Ŀ�ģʽ��CABAC��)
*   ���ֿ�ԴH264����������(http://blog.csdn.net/sunshine1314/article/details/712401)
*     �����ٶȣ�CoreAVC decoder > ffmpeg > IPP simple player > x264 decoder > t264 decoder > jm86 decoder
*     1.JM decoder -- H.264�Ĺٷ�Դ��(У��ģ��),֧�����Ժã���ʵ���Բ�
*     2.T264 decoder -- �й��Ŀ�Դ��Ŀ����������Ż���������ֻ�ܽ�T264���������ɵ�������
*     3.x264 decoder -- (ffmpeg �����ڳ���?)
*     4.ffmpeg libavcodec -- 
*     5.Intel IPP(Integrated Performance Primitives)

*********************************************************************************************/

// ���RGBȻ��ת����ת��ΪYUV  
#if 0
avpicture_fill((AVPicture*)m_pRGBFrame, (uint8_t*)rgb_buff, PIX_FMT_RGB24, nWidth, nHeight);  
m_pRGBFrame->data[0]  += m_pRGBFrame->linesize[0] * (nHeight - 1);  
m_pRGBFrame->linesize[0] *= -1;                     
m_pRGBFrame->data[1]  += m_pRGBFrame->linesize[1] * (nHeight / 2 - 1);  
m_pRGBFrame->linesize[1] *= -1;  
m_pRGBFrame->data[2]  += m_pRGBFrame->linesize[2] * (nHeight / 2 - 1);  
m_pRGBFrame->linesize[2] *= -1; 
sws_scale(scxt,m_pRGBFrame->data,m_pRGBFrame->linesize,0,c->height,m_pYUVFrame->data,m_pYUVFrame->linesize);
#endif