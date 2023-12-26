#include "rtklib.h"
#define RTCM3PREAMB 0xD3        /* rtcm ver.3 frame preamble */
#define BUFFSIZE 4096
#define BUFFMULTI 5
static int udi2int(int udi){
	return 0;
}
static int outssr_cbias(ssr_t* ssr,char* buff, int ssrnum){
	char* p = (char*)buff;
	char subbuff[32];
	double ep[6];
	int epup = 0,codeup=0;
	int satnum = 0,i,j,udi=0;
	for(i = 0; i < ssrnum; ++i){
		if (ssr[i].update == 1) {
			satnum += 1;
			udi = udi2int(ssr[i].udi[4]);
			if (!epup)
			{
				time2epoch(ssr[i].t0[4], ep); epup = 1;
			}
		}
	}
	if (satnum <= 0) return 1;
	p += sprintf(p, "> CODE_BIAS %4d %02d %02d %02d %02d %04.1f %2d %3d FILE\n",
		(int)ep[0], (int)ep[1], (int)ep[2], (int)ep[3], (int)ep[4],ep[5], udi,satnum);
	for(i = 0; i < ssrnum; ++i){
		if(ssr[i].update == 1){
			satno2id(i, subbuff);
			
			codeup = 0;
			for(j = 0;j < MAXCODE; ++j){
				if(ssr[i].cbias[j] != 0.0){
					codeup += 1;
				} 
			}
			p += sprintf(p, "%s%5d", subbuff, codeup);
			for (j = 0; j < MAXCODE; ++j) {
				if (ssr[i].cbias[j] != 0.0) {
					p += sprintf(p, "%6s%10.4f", code2obs(j), ssr[i].cbias[j]);
				}
			}
			if (p - (char*)buff > BUFFSIZE * BUFFMULTI) return 0;
			p+=sprintf(p, "\n");
			ssr[i].update = 0;
		}
	}
	return 1;
}

static int outssr_orb(ssr_t* ssr, char* buff, int ssrnum) {
	char* p = (char*)buff;
	char subbuff[32];
	double ep[6];
	int epup = 0;
	int satnum = 0, i, j, udi = 0;
	for (i = 0; i < ssrnum; ++i) {
		if (ssr[i].update == 1) {
			satnum += 1;
			udi = udi2int(ssr[i].udi[0]);
			if(!epup)
			{
				time2epoch(ssr[i].t0[0], ep); epup = 1;
			}
		}
	}
	if (satnum <= 0) return 1;
	p += sprintf(p, "> ORBIT %4d %02d %02d %02d %02d %04.1f %2d %3d FILE\n",
		(int)ep[0], (int)ep[1], (int)ep[2], (int)ep[3], (int)ep[4], ep[5], udi, satnum);
	for (i = 0; i < ssrnum; ++i) {
		if (ssr[i].update == 1) {
			satno2id(i, subbuff);
			p += sprintf(p, "%s%5d%10.4f%10.4f%10.4f%10.4f%10.4f%10.4f\n", subbuff, ssr[i].iod[0],
				ssr[i].deph[0], ssr[i].deph[1], ssr[i].deph[2],
				ssr[i].ddeph[0], ssr[i].ddeph[1], ssr[i].ddeph[2]);
			if (p - (char*)buff > BUFFSIZE * BUFFMULTI) return 0;
			ssr[i].update = 0;
		}
		
	}
	return 1;
}

static int outssr_clk(ssr_t* ssr, char* buff, int ssrnum) {
	char* p = (char*)buff;
	char subbuff[32];
	double ep[6];
	int epup = 0;
	int satnum = 0, i, j, udi = 0;
	for (i = 0; i < ssrnum; ++i) {
		if (ssr[i].update == 1) {
			satnum += 1;
			udi = udi2int(ssr[i].udi[1]);
			if (!epup)
			{
				time2epoch(ssr[i].t0[1], ep); epup = 1;
			}
		}
	}
	if (satnum <= 0) return 1;
	p += sprintf(p, "> CLOCK %4d %02d %02d %02d %02d %04.1f %2d %3d FILE\n",
		(int)ep[0], (int)ep[1], (int)ep[2], (int)ep[3], (int)ep[4], ep[5], udi, satnum);
	for (i = 0; i < ssrnum; ++i) {
		if (ssr[i].update == 1) {
			satno2id(i, subbuff);
			p += sprintf(p, "%s%5d%10.4f%10.4f%10.4f\n", subbuff, ssr[i].iod[1],
				ssr[i].dclk[0], ssr[i].dclk[1], ssr[i].dclk[2]);
			if (p - (char*)buff > BUFFSIZE * BUFFMULTI) return 0;
			ssr[i].update = 0;
		}
		
	}
	return 1;
}

static int outssr_orbclk(ssr_t* ssr, char* buff, int ssrnum) {
	char* p = (char*)buff;
	char subbuff[32];
	double ep[6];
	int epup = 0;
	int satnum = 0, i, j, udi = 0;
	for (i = 0; i < ssrnum; ++i) {
		if (ssr[i].update == 1) {
			satnum += 1;
			udi = udi2int(ssr[i].udi[1]);
			if (!epup)
			{
				time2epoch(ssr[i].t0[1], ep); epup = 1;
			}
		}
	}
	if (satnum <= 0) return 1;
	p += sprintf(p, "> CLOCK %4d %02d %02d %02d %02d %04.1f %2d %3d FILE\n",
		(int)ep[0], (int)ep[1], (int)ep[2], (int)ep[3], (int)ep[4], ep[5], udi, satnum);
	for (i = 0; i < ssrnum; ++i) {
		if (ssr[i].update == 1) {
			satno2id(i, subbuff);
			p += sprintf(p, "%s%5d%10.4f%10.4f%10.4f\n", subbuff,ssr[i].iod[1], 
				ssr[i].dclk[0], ssr[i].dclk[1], ssr[i].dclk[2]);
			if (p - (char*)buff > BUFFSIZE * BUFFMULTI) return 0;
		}
	}

	p += sprintf(p, "> ORBIT %4d %02d %02d %02d %02d %04.1f %2d %3d FILE\n",
		(int)ep[0], (int)ep[1], (int)ep[2], (int)ep[3], (int)ep[4], ep[5], udi, satnum);
	for (i = 0; i < ssrnum; ++i) {
		if (ssr[i].update == 1) {
			satno2id(i, subbuff);
			p += sprintf(p, "%s%5d%10.4f%10.4f%10.4f%10.4f%10.4f%10.4f\n", subbuff, ssr[i].iod[0], 
				ssr[i].deph[0], ssr[i].deph[1], ssr[i].deph[2],
				ssr[i].ddeph[0], ssr[i].ddeph[1], ssr[i].ddeph[2]);
			if (p - (char*)buff > BUFFSIZE * BUFFMULTI) return 0;
			ssr[i].update = 0;
		}
		
	}
	return 1;
}

static int outssr_obsmsm5(const obsd_t* obs,const int n, char* buff) {
	char* p = (char*)buff;
	char subbuff[32];
	int i = 0,j;
	double ep[6];
	if (n <= 0) {
		p += sprintf(p, "> OBS NO DATA\n");
		return;
	}
	time2epoch(obs[0].time, ep);
	p += sprintf(p, "> OBS %4d %02d %02d %02d %02d %04.1f %3d LCDS FILE\n",
		(int)ep[0], (int)ep[1], (int)ep[2], (int)ep[3], (int)ep[4], ep[5], n);
	for (i = 0; i < n; ++i) {
		satno2id(obs[i].sat, subbuff);
		p += sprintf(p, "%s  ",subbuff);
		for (j = 0; j < NFREQ; ++j) {
			if (obs[i].L[j] == 0.0 && obs[i].P[j] == 0.0 && obs[i].D[j] == 0.0 && obs[i].SNR[j] == 0.0) {
				continue;
			}
			p+=sprintf(p, "%s %14.3f %14.3f %14.3f %10.3f; ", code2obs(obs[i].code[j]), 
				obs[i].L[j], obs[i].P[j], obs[i].D[j], obs[i].SNR[j]*SNR_UNIT);
			if (p - (char*)buff > BUFFSIZE * BUFFMULTI) return 0;
		}
		p += sprintf(p, "\n");
	}
	return 1;
}

static int outssr_brdc(const nav_t* nav,const int ephsat, const int ephset, char* buff) {
	char* p = (char*)buff;
	char subbuff[32];
	double ep[6],ep1[6];
	int sys,prn,iode;
	eph_t* eph;
	geph_t* geph;
	sys = satsys(ephsat, &prn);
	satno2id(ephsat, subbuff);
	if (sys == SYS_GLO) {
		geph = nav->geph + prn - 1;
		time2epoch(geph->toe, ep);
		time2epoch(geph->tof, ep1);
		p += sprintf(p, "> BRDC %4d %02d %02d %02d %02d %04.1f FILE %s IODE:%d TOF:%4d %02d %02d %02d %02d %04.1f\n",
			(int)ep[0], (int)ep[1], (int)ep[2], (int)ep[3], (int)ep[4], ep[5], subbuff,geph->iode,
			(int)ep1[0], (int)ep1[1], (int)ep1[2], (int)ep1[3], (int)ep1[4], ep1[5]);
	}
	else {
		eph = nav->eph + ephsat - 1 + MAXSAT * ephset;
		iode = sys==SYS_CMP? ((int)eph->toe.time / 720) % 120 : eph->iode;
		time2epoch(eph->toe, ep);
		time2epoch(eph->toc, ep1);
		p += sprintf(p, "> BRDC %4d %02d %02d %02d %02d %04.1f FILE %s IODE:%d TOC:%4d %02d %02d %02d %02d %04.1f CODE:%X\n",
			(int)ep[0], (int)ep[1], (int)ep[2], (int)ep[3], (int)ep[4], ep[5], subbuff, eph->iode,
			(int)ep1[0], (int)ep1[1], (int)ep1[2], (int)ep1[3], (int)ep1[4], ep1[5],(unsigned int)eph->code);
	}
	if (p - (char*)buff > BUFFSIZE * BUFFMULTI) return 0;
	return 1;
}

int main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("arg error, should be <ssrfilepath> <outpath> ");
		return 0;
	}
	rtcm_t rtcm;
	init_rtcm(&rtcm);
	FILE* fpin = NULL, *fpout = NULL;
	uint8_t databuff[BUFFSIZE];
	int rtcmtype = 0,ret,nr,i;
	unsigned int totalnr = 0;
	char* longbuff = NULL;
	double refep[6] = { 2023,12,22,0,0,0 };
	double epcur[6];
	char rcvtime[32];
	rtcm.time = epoch2time(refep);
	if (!(fpin = fopen(argv[1], "rb"))) {
		printf("infile <%s> open error", argv[1]);
		return 0;
	}
	if (!(fpout = fopen(argv[2], "w"))) {
		printf("outfile <%s> open error", argv[2]);
		return 0;
	}
	
	if (!(longbuff = (char*)malloc(sizeof(char) * BUFFSIZE * BUFFMULTI))) {
		return 0;
	}
	rtcm.nbyte = 0;
	totalnr = 0;
	//while ( 0 < (nr = (int)fread(buff, sizeof(char), BUFFSIZE, fpin))) {
	while(!feof(fpin)){
		nr = fread(databuff, sizeof(char), BUFFSIZE, fpin);
		
		totalnr += nr;
		if (totalnr > 1073741824u) {
			break;
		}
		
		for (i = 0; i < nr; ++i) {
			if (rtcm.nbyte == 0) {
				if (databuff[i] != RTCM3PREAMB)
				{
					continue;
				}
				rtcm.buff[rtcm.nbyte++] = databuff[i];
				continue;
			}
			rtcm.buff[rtcm.nbyte++] = databuff[i];

			if (rtcm.nbyte == 3) {
				rtcm.len = getbitu(rtcm.buff, 14, 10) + 3; /* length without parity */
			}
			if (rtcm.nbyte == 5) {
				rtcmtype = getbitu(rtcm.buff, 24, 12);
			}
			if (rtcm.nbyte < 3 || rtcm.nbyte < rtcm.len + 3)
			{
				continue;
			}
			rtcm.nbyte = 0;

			/* check parity */
			if (rtk_crc24q(rtcm.buff, rtcm.len) != getbitu(rtcm.buff, rtcm.len * 8, 24)) {
				trace(2, "rtcm3 parity error: len=%d\n", rtcm.len);
				continue;
			}
			/* decode rtcm3 message */
			ret = decode_rtcm3(&rtcm);

			time2epoch(rtcm.time, epcur);
			if (ret <= 0) {
				continue;
			}

			//printf("read %d\n", rtcmtype);
			strcpy(rcvtime, time_str(rtcm.time, 3));
			printf("%s\r", rcvtime);
			//code bias
			fprintf(fpout, "%% %s\n", rcvtime);
			if (rtcmtype == 1059 || rtcmtype == 1065 || rtcmtype == 1242 || rtcmtype == 1260) {
				if (!outssr_cbias(rtcm.ssr, longbuff, MAXSAT)) {
					printf("message out of buffsize \n");
				}
				else {
					fprintf(fpout, "%s", longbuff);
				}
			}
			//orbit
			else if (rtcmtype == 1057 || rtcmtype == 1063 || rtcmtype == 1240 || rtcmtype == 1258) {
				if (!outssr_orb(rtcm.ssr, longbuff, MAXSAT)) {
					printf("message out of buffsize \n");
				}
				else {
					fprintf(fpout, "%s", longbuff);
				}
			}
			//clk
			else if (rtcmtype == 1058 || rtcmtype == 1064 || rtcmtype == 1241 || rtcmtype == 1259) {
				if (!outssr_clk(rtcm.ssr, longbuff, MAXSAT)) {
					printf("message out of buffsize \n");
				}
				else {
					fprintf(fpout, "%s", longbuff);
				}
			}
			//combined orbit and clk
			else if (rtcmtype == 1060 || rtcmtype == 1066 || rtcmtype == 1243 || rtcmtype == 1261) {
				if (!outssr_orbclk(rtcm.ssr, longbuff, MAXSAT)) {
					printf("message out of buffsize \n");
				}
				else {
					fprintf(fpout, "%s", longbuff);
				}
			}
			//observation  msm5 
			else if (rtcmtype == 1075 || rtcmtype == 1085 || rtcmtype == 1095 || rtcmtype == 1125) {
				if (!outssr_obsmsm5(rtcm.obs.data, rtcm.obs.n, longbuff)) {
					printf("message out of buffsize \n");
				}
				else {
					fprintf(fpout, "%s", longbuff);
				}
			}
			else if (rtcmtype == 1019 || rtcmtype == 1020 || rtcmtype == 1042 || rtcmtype == 1045 || rtcmtype == 1046) {
				if (!outssr_brdc(&rtcm.nav, rtcm.ephsat, rtcm.ephset, longbuff)) {
					printf("message out of buffsize \n");
				}
				else {
					fprintf(fpout, "%s", longbuff);
				}
			}
		}
	}
	fclose(fpin);
	fclose(fpout);
	free_rtcm(&rtcm);
	free(longbuff);
	return 1;
}

