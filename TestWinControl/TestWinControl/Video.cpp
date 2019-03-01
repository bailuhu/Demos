#include "stdafx.h"

#include<windows.h>
#include <vfw.h>
#pragma comment(lib,"vfw32.lib")

/*
const int nWidth = 100 ;	// 100
const int nHeight = 100 ;	// 100
int main(int argc, char* argv[])
{
	int frames = 1 ;

	char buffer[nWidth*nHeight*4] ;
	memset(buffer, 1, sizeof(buffer));
	AVIFileInit();   //初始化AVIFILE函数库
	PAVIFILE m_pAviFile;
	char fileName[]="D:\\000\\桌面\\a.avi" ;
	PAVISTREAM m_pAviStream;
	if( AVIFileOpen(&m_pAviFile, fileName, OF_CREATE|OF_WRITE, NULL) )
	{// 打开AVI文件,通过AviFileInfo获得AVI文件信息
		MessageBox(NULL,"AviFileOpen error","error",MB_OK);
		return 0;
	}

	AVISTREAMINFO m_AviStreamInfo;
	memset(&m_AviStreamInfo, 0, sizeof(m_AviStreamInfo));
	m_AviStreamInfo.fccType = streamtypeVIDEO;
	m_AviStreamInfo.dwRate = 1 ;
	m_AviStreamInfo.dwScale = 1 ;
	m_AviStreamInfo.dwSuggestedBufferSize  = nHeight * nWidth * 32 ;
	m_AviStreamInfo.rcFrame.bottom = nHeight ;
	m_AviStreamInfo.rcFrame.right = nWidth ;
	strcpy(m_AviStreamInfo.szName, fileName);

	AVIFileCreateStream(m_pAviFile, &m_pAviStream, &m_AviStreamInfo);


	AVICOMPRESSOPTIONS  m_Options; 
	LPAVICOMPRESSOPTIONS pOptions;
	pOptions = &m_Options;
	memset(&m_Options, 0, sizeof(m_Options));


// 	AVICOMPRESSOPTIONS  m_Options; 
// 	AVICOMPRESSOPTIONS * pOptions[1] = {&m_Options};
//	memset(&m_Options, 0, sizeof(m_Options));
	//AVISaveOptions(NULL,0,1,&m_pAviStream,pOptions);
	DWORD params = 75;
	// char a[4];
	// memcpy(a,&m_Options.fccHandler,4);
	m_Options.fccType = ICTYPE_VIDEO ;
	// mmioFOURCC('i','v','4','1'); mmioFOURCC('m','p','4','2'); mmioFOURCC('M','P','G','4');
	m_Options.fccHandler = mmioFOURCC('i','v','4','1');
	m_Options.dwQuality = 2000;
	m_Options.dwFlags = 8 ;
	m_Options.lpParms = 0;
	m_Options.cbParms = 0 ;
	PAVISTREAM m_pCpsStream;
	AVIMakeCompressedStream(&m_pCpsStream, m_pAviStream, pOptions, NULL);
	BITMAPINFOHEADER bmpInfoHeader;
	memset(&bmpInfoHeader, 0, sizeof(bmpInfoHeader));
	bmpInfoHeader.biSize = sizeof(bmpInfoHeader);
	bmpInfoHeader.biHeight = nHeight;
	bmpInfoHeader.biWidth = nWidth ;
	bmpInfoHeader.biBitCount = 32 ;
	bmpInfoHeader.biCompression = BI_RGB ;
	bmpInfoHeader.biPlanes = 1 ;
	COMPVARS m_Com;
	ICCompressorChoose(NULL,ICMF_CHOOSE_KEYFRAME,&bmpInfoHeader, NULL,&m_Com,_T("选择压缩类型"));

	if(0 != AVIStreamSetFormat(m_pCpsStream, 0, &bmpInfoHeader, sizeof(bmpInfoHeader)))
	{
		MessageBox(NULL,"setformat","a;lsdkf",MB_OK);
		return 0;
	}

	// AVIStreamSetFormat(m_pAviStream,0,&bmpInfoHeader,sizeof(bmpInfoHeader));
	for( int i = 0 ;i < 1000 ;i++)
	{ 
		// AVIStreamWrite(m_pAviStream,frames*5,1,buffer,sizeof(buffer),AVIIF_KEYFRAME,NULL,NULL);
		AVIStreamWrite(m_pCpsStream, frames, 1, buffer, sizeof(buffer), AVIIF_KEYFRAME, NULL, NULL);
		memset(buffer, i%256, sizeof(buffer));
		frames++ ;
	}
	// m_pCpsStream->Release();
	if(m_pAviStream)
	{
		m_pAviStream->Release();
		m_pAviStream = NULL; 
	}
	if(m_pAviFile)
	{
		AVIFileClose(m_pAviFile);
		m_pAviFile->Release();
		m_pAviFile = NULL;
	}
	AVIFileExit();   //释放库
	return 0;
}
*/


/*
//生成avi
void BMPtoAVI(CString szAVIName, CString strBmpDir)
{
　CFileFind finder;
　strBmpDir += _T("\\*.*"); 
　AVIFileInit(); 
　AVISTREAMINFO strhdr;
　PAVIFILE pfile;
　PAVISTREAM ps; 
　int nFrames =0; 
　HRESULT hr; 

　BOOL bFind = finder.FindFile(strBmpDir);
　while(bFind)
　{
　　bFind = finder.FindNextFile();
　　if(!finder.IsDots() && !finder.IsDirectory())
　　{
　　　CString str = finder.GetFilePath();
　　　FILE *fp = fopen(str,"rb");
　　　BITMAPFILEHEADER bmpFileHdr;
　　　BITMAPINFOHEADER bmpInfoHdr;
　　　fseek( fp,0,SEEK_SET);
　　　fread(&bmpFileHdr,sizeof(BITMAPFILEHEADER),1, fp);
　　　fread(&bmpInfoHdr,sizeof(BITMAPINFOHEADER),1, fp);

　　　BYTE *tmp_buf = NULL;
　　　if(nFrames ==0 )
　　　{
　　　　AVIFileOpen(&pfile,szAviName,OF_WRITE | OF_CREATE,NULL);
　　　　_fmemset(&strhdr, 0, sizeof(strhdr));
　　　　strhdr.fccType = streamtypeVIDEO;// stream type
　　　　strhdr.fccHandler = 0;
　　　　strhdr.dwScale = 1;
　　　　strhdr.dwRate = 15; // 15 fps
　　　　strhdr.dwSuggestedBufferSize = bmpInfoHdr.biSizeImage ;
　　　　SetRect(&strhdr.rcFrame, 0, 0, bmpInfoHdr.biWidth, bmpInfoHdr.biHeight);

　　　　// And create the stream;
　　　　hr = AVIFileCreateStream(pfile,&ps,&strhdr); 
　　　　// hr = AVIStreamSetFormat(ps,nFrames,&bmpInfoHdr,sizeof(bmpInfoHdr));
　　　}
　　　tmp_buf = new BYTE[bmpInfoHdr.biWidth * bmpInfoHdr.biHeight * 3];
　　　fread(tmp_buf, 1, bmpInfoHdr.biWidth * bmpInfoHdr.biHeight * 3, fp);
　　　hr = AVIStreamSetFormat(ps,nFrames,&bmpInfoHdr,sizeof(bmpInfoHdr));
　　　hr = AVIStreamWrite(ps, // stream pointer
　　　　　　nFrames , // time of this frame
　　　　　　1, // number to write
　　　　　　(LPBYTE) tmp_buf,
　　　　　　bmpInfoHdr.biSizeImage , // size of this frame
　　　　　　AVIIF_KEYFRAME, // flags....
　　　　　　NULL,
　　　　　　NULL);

　　　nFrames ++; 
　　　fclose(fp);
　　}
　}

　AVIStreamClose(ps);

　if(pfile != NULL)
　　AVIFileRelease(pfile);
　AVIFileExit();
}
*/

// 1、视频压缩的初始化：   
COMPVARS m_CV;
DWORD m_OutFormatSize, m_OutBufferSize;
BITMAPINFO* m_pOutInfo, m_InInfo;

// 1、视频压缩的初始化：   
void AviFile::InitCompressor(BITMAPINFO bmpInfo)   
{   
	m_InInfo = bmpInfo;
	memset(&m_CV, 0, sizeof(COMPVARS));   
	m_CV.dwFlags = ICMF_COMPVARS_VALID;   
	m_CV.cbSize = sizeof(m_CV);   
	m_CV.cbState = 0;   
	m_CV.fccHandler = mmioFOURCC('d','i','v','x');
	// 注：如果是调用XIVD，则上面的参数设为mmioFOURCC('x','v','i','d');  
	m_CV.fccType = ICTYPE_VIDEO;   
	m_CV.hic = ICOpen(ICTYPE_VIDEO, mmioFOURCC('d','i','v','x'), ICMODE_COMPRESS);
	// 注：如果是调用XIVD，则上面的参数设为mmioFOURCC('x','v','i','d');  
	m_CV.lDataRate=780;   
	m_CV.lFrame=0;   
	m_CV.lKey=15;   
	m_CV.lKeyCount=0;   
	m_CV.lpbiIn=NULL;   
	m_CV.lpBitsOut=NULL;   
	m_CV.lpBitsPrev=m_CV.lpState=NULL;   
	m_CV.lQ=ICQUALITY_DEFAULT;   
	if(m_CV.hic!=NULL)   
	{   // 获得压缩后数据的格式
		m_OutFormatSize = ICCompressGetFormatSize(m_CV.hic, &m_InInfo);   
		m_pOutInfo = (BITMAPINFO*)new BYTE[m_OutFormatSize];   
		ICCompressGetFormat(m_CV.hic, &m_InInfo, m_pOutInfo);   
		m_OutBufferSize = ICCompressGetSize(m_CV.hic, &m_InInfo, m_pOutInfo);

		// 准备编码器  
		if(ICSeqCompressFrameStart(&m_CV, &m_InInfo))   
			AfxMessageBox(_T("压缩初始化成功"));   
		else   
			AfxMessageBox(_T("压缩初始化失败"));   
		if(ICDecompressBegin(m_CV.hic,m_pOutInfo, &m_InInfo) != ICERR_OK)   
			AfxMessageBox(_T("解压初始化失败！"));   
		else   
			AfxMessageBox(_T("解压初始化成功！"));   
	}   
}   

// 2、VFW中视频采集中的回调函数（我用的是摄相头）：   
LRESULT CALLBACK AviFile::FrameCallBack(HWND hWnd, LPVIDEOHDR lpVHdr)   
{   
	AviFile* pDlg=(AviFile*)::AfxGetMainWnd();   
	pDlg->CompressFrame(lpVHdr);// 压缩一帖视频数据
	return   TRUE;   
}   
// 3、视频压缩并解压   
void  AviFile::CompressFrame(LPVOID lpData)   
{   
	BOOL bKeyFrame;   
	m_OutActSize = this->m_InInfo.bmiHeader.biSizeImage;

	// 压缩数据
	BYTE* Buf=(BYTE*)ICSeqCompressFrame(&m_CV, 0, lpData, &bKeyFrame, (long*)&m_OutActSize);   

	if(Buf != NULL)   
	{   
		DWORD  Result;  
		// 解压缩数据　
		Result = ICDecompress（m_CV.hic,//解码器的句柄
			　　　　　　　　　ICDECOMPRESS_HURRYUP,//压缩方式
							&m_InInfo.bmiHeader,//输入数据格式信息
							 Buf,//输入数据缓冲区
							&m_pOutInfo->bmiHeader,//解压后数据的格式信息
							 buff);//解压后的数据存入地址 
		//if(Result==ICERR_OK)   
		AfxMessageBox(_T("解压成功！"));   
		//else   
		//AfxMessageBox("解压失败！");   
		CDC   *pdc=GetDC();   
		HDC   hdc=pdc->GetSafeHdc();   
		StretchDIBits(hdc,0,0,176,144,0,0,176,144,buff,&m_InInfo),DIB_RGB_COLORS,SRCCOPY);   
	}   
}   