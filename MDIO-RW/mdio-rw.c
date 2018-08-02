#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/syslog.h> 
#include <stdio.h> 
#include <sys/ioctl.h> 
#include <net/if.h>
#include <linux/mii.h>
#include <linux/sockios.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#define USAGE "mdio-rw <-i devName> <-r <numReg> |-w <regVal> <-p phyAddr> <-a regAddr>"

struct ifreq ifr;
int sockfd; 
char devName[64];

int MiiInit(void) 
{
	struct mii_ioctl_data *mii;

	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_ifrn.ifrn_name, devName); //set to whatever your ethernet device is
	mii = (struct mii_ioctl_data *)(&ifr.ifr_ifru.ifru_data);
	mii->phy_id = 1; //set to your phy's ID
	mii->reg_num = 0; //the register you want to read
	mii->val_in = 0;
	mii->val_out = 0;

	//sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd < 0) {
	syslog(LOG_ERR | LOG_USER, "Can't create socket for MII, error: %s", "bogus");
	return -1;
	}

	return 0;
}

int MiiRead (unsigned int portNumber , unsigned int MIIReg, unsigned int* value)
{
	struct mii_ioctl_data *mii;

	mii = (struct mii_ioctl_data *)(&ifr.ifr_ifru.ifru_data);
	mii->phy_id = portNumber;
	mii->reg_num = MIIReg;
	if (ioctl(sockfd, SIOCGMIIREG, &ifr) < 0)
	{
		perror("ioctl[SIOCGMIIPHY]");
		return -1;
	}
	*value = (unsigned int)(mii->val_out & 0xffff);

	return 0;
}

int MiiWrite (unsigned int portNumber , unsigned int MIIReg, unsigned int value)
{
	struct mii_ioctl_data *mii;

	mii = (struct mii_ioctl_data *)(&ifr.ifr_ifru.ifru_data);
	mii->phy_id = portNumber;
	mii->reg_num = MIIReg;
	mii->val_in = value & 0xffff;
	if (ioctl(sockfd, SIOCSMIIREG, &ifr) < 0) {
		perror("ioctl[SIOCSMIIPHY]");
		return -1;
	}
	return 0;
}

int
main (int argc, char **argv)
{
	int iflag = 0;
	int rflag = 0;
	unsigned int rvalue;
	int wflag = 0;
	unsigned int wvalue;
	int pflag = 0;
	unsigned int pvalue = 0;
	int aflag = 0;
	unsigned int avalue = 0;
	int i;
	int c;
	unsigned int value;

	opterr = 0;


	while ((c = getopt (argc, argv, "i:r:w:p:a:")) != -1)
		switch (c)
		{
			case 'i':
				strcpy(devName, optarg);
				iflag = 1;
				break;
			case 'r':
				sscanf(optarg,"%x",&rvalue);
				rflag = 1;
				break;
			case 'w':
				sscanf(optarg,"%x",&wvalue);
				wflag = 1;
				break;
			case 'p':
				sscanf(optarg,"%x",&pvalue);
				pflag = 1;
				break;
			case 'a':
				sscanf(optarg,"%x",&avalue);
				aflag = 1;
				break;
			case '?':
				if (optopt == 'r' || optopt == 'w' || optopt == 'p' || optopt == 'a')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,
						"Unknown option character `\\x%x'.\n",
						optopt);
				return 1;
			default:
				abort ();
		}
     
	 if(rflag && wflag) {
		 printf("Must Pick -r OR -w. Not both\n");
		 printf("%s\n",USAGE);
		 return 1;
	 }

	 if(! (rflag || wflag)) {
		 printf("Must Pick -r OR -w\n");
		 printf("%s\n",USAGE);
		 return 1;
	 }

	 if(! (pflag && aflag)) {
		 printf("Must set -p AND -a \n");
		 printf("%s\n",USAGE);
		 return 1;
	 }

	 if(! iflag){
		 printf("Must set -i\n");
		 printf("%s\n",USAGE);
		 return 1;
	 }

	sprintf(ifr.ifr_name, "eth0");
  
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if(sockfd < 0){ 
		perror("socket"); 
		exit(1); 
	} 

	MiiInit();

//	printf("r : flag %d, val %0x\n",rflag,rvalue);
//	printf("w : flag %d, val %0x\n",wflag,wvalue);
//	printf("p : flag %d, val %0x\n",pflag,pvalue);
//	printf("a : flag %d, val %0x\n",aflag,avalue);
	printf("## %s Phy %0x ##\n",devName,pvalue);

	 if(rflag) {
		for(i=0;i<rvalue;i++) {
			MiiRead (pvalue , avalue, &value);
  			printf("RegAddr %04X : %08X\n",avalue,value); 
			avalue++;
		}
	 }
	 else {
		printf("write: PhyAddr 0x%x RegAddr 0x%x RegVal 0x%x\n",pvalue,avalue,wvalue);
		MiiWrite (pvalue , avalue, wvalue);
	 }

     
	return 0;
}
