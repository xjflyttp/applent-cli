#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hidapi.h"

class RequestCommand
{
	public:
		uint cSize;
		char sHeader[24];
		char sPara[28];
		uint nSignature;
		uint nChecksum;
		
		RequestCommand() 
		{
			this->cSize = 60;
			memset(this->sHeader, 0, 24);
			memset(this->sPara, 0, 28);
			this->nSignature = 0x88805550;
			this->nChecksum = 0;
		}
		
		void setHeader(const char* str) 
		{
			strcpy(this->sHeader, str);
		}
		
		void setPara(const char* str) 
		{
			strcpy(this->sPara, str);
		}

		void setupChecksum()
		{
			unsigned char buff[64];
			unsigned int sum = 0;
			unsigned int indexBuff = 60;
			unsigned int index =0 ;
			
			this->getData(buff);
			
			for (; index<60; index++) {
				sum += (int)buff[index];
			}
			
			this->nChecksum = sum;
		};
	
		void getData(unsigned char* data)
		{
			unsigned char buff[64];
			memset(buff, 0, 64);
			unsigned int indexBuff = 0;
			
			buff[indexBuff++] = (unsigned char)(0xff & this->cSize);
			buff[indexBuff++] = (unsigned char)((0xff00 & this->cSize) >> 8);
			buff[indexBuff++] = (unsigned char)((0xff0000 & this->cSize) >> 16);
			buff[indexBuff++] = (unsigned char)((0xff000000 & this->cSize) >> 24);
			
			for (int hIndex = 0; hIndex<24; hIndex++) {
				buff[indexBuff++] = this->sHeader[hIndex];
			}
			
			for (int pIndex = 0; pIndex<28; pIndex++) {
				buff[indexBuff++] = this->sPara[pIndex];
			}
			
			buff[indexBuff++] = (unsigned char)(0xff & this->nSignature);
			buff[indexBuff++] = (unsigned char)((0xff00 & this->nSignature) >> 8);
			buff[indexBuff++] = (unsigned char)((0xff0000 & this->nSignature) >> 16);
			buff[indexBuff++] = (unsigned char)((0xff000000 & this->nSignature) >> 24);
			
			buff[indexBuff++] = (unsigned char)(0xff & this->nChecksum);
			buff[indexBuff++] = (unsigned char)((0xff00 & this->nChecksum) >> 8);
			buff[indexBuff++] = (unsigned char)((0xff0000 & this->nChecksum) >> 16);
			buff[indexBuff++] = (unsigned char)((0xff000000 & this->nChecksum) >> 24);

			memcpy(data, buff, sizeof(buff));
		};
};

class ATCommander
{
	public: 
		void init(unsigned int vid, unsigned int pid)
		{
			this->vid = vid;
			this->pid = pid;
		}
		
		void cmd(const char* header, const char* para, unsigned char* data)
		{
			hid_device *handle;
			int result;
			unsigned char buff[64];
			memset(buff, 0, 64);

			RequestCommand request;
			request.setHeader(header);
			request.setPara(para);
			request.setupChecksum();
			request.getData(buff);
			
			result = hid_init();
			if (result < 0) {
				return;
			}
			
			handle = hid_open(this->vid, this->pid, NULL);
			
			result = hid_write(handle, buff, 64);
			
			result = hid_read_timeout(handle, data, 64, 1000);

			hid_close(handle);
			hid_exit();
		}
	
	private:
		unsigned int vid;
		unsigned int pid;
		
};

int main(int argc, char* argv[])
{
	
	if (argc < 3) {
		printf("%s Header Para\n", argv[0]);
		return 0;
	}

	const unsigned int VID = 0x825;
	const unsigned int PID = 0x826;
	
	char *header = argv[1];
	char *para = argv[2];
	
	unsigned char buff[64];
	memset(buff, 0, 64);
	ATCommander atc;
	atc.init(VID, PID);
	atc.cmd(header, para, buff);
	
	printf("%s\n", buff);
	
	return 0;
}