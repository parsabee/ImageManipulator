#include <iostream>
#include <string.h>
#include "filters.h"
#include "PNMreader.h"
#include "PNMwriter.h"

#define USAGE "\nUsage: ./image_manipulator input.pnm output.pnm filter\nHelp: ./image_distorter --help\n"

int outputCheck(char *s)
{   
    /*check if the out put is .pnm*/
    char *arg = strdup(s);
    int check = 1;
    int i;
    int len = strlen(arg);
    std::string pnm = "pnm";
    for(i=0; i<3; i++){
        if(arg[len-(3-i)] != pnm[i]){
            check = 0;
            break;
        }
    }
    free(arg);
    return check;
}

void _330(char **argv){
	PNMreader reader(argv[1]);
    Shrinker sr;
    LRConcat lr1;
    LRConcat lr2;
    LRConcat lr3;
    LRConcat lr4;
    LRConcat lr5;
    LRConcat lr6;
    TBConcat tb1;
    TBConcat tb2;
    TBConcat tb3;
    Blur br1;
    Blur br2;
    Blur br3;
    Subtract sb;
    Rotate rt1;
    Rotate rt2;
    Rotate rt3;
    Rotate rt4;
    Mirror mr;
    Grayscale gs;

    Color green(893, 672, 10, 64, 40);
    Color gold(893, 672, 255, 215, 0);

    br1.SetInput(reader.GetOutput());
    sr.SetInput(br1.GetOutput());

    rt1.SetInput(sr.GetOutput());
    rt2.SetInput(rt1.GetOutput());
    br2.SetInput(rt2.GetOutput());
    rt3.SetInput(br2.GetOutput());
    rt4.SetInput(rt3.GetOutput());
    br3.SetInput(rt4.GetOutput());

    lr1.SetInput(sr.GetOutput());
    lr1.SetInput2(rt2.GetOutput());
    lr2.SetInput(br2.GetOutput());
    lr2.SetInput2(br3.GetOutput());
    tb1.SetInput(lr1.GetOutput());
    tb1.SetInput2(lr2.GetOutput());

    sb.SetInput(br1.GetOutput());
    sb.SetInput2(reader.GetOutput());

    lr3.SetInput(tb1.GetOutput());
    lr3.SetInput2(sb.GetOutput());

    mr.SetInput(sb.GetOutput());

    gs.SetInput(sr.GetOutput());

    lr4.SetInput(gs.GetOutput());
    lr4.SetInput2(green.GetOutput());
    lr5.SetInput(gold.GetOutput());
    lr5.SetInput2(gs.GetOutput());
    tb2.SetInput(lr4.GetOutput());
    tb2.SetInput2(lr5.GetOutput());

    lr6.SetInput(mr.GetOutput());
    lr6.SetInput2(tb2.GetOutput());

    tb3.SetInput(lr3.GetOutput());
    tb3.SetInput2(lr6.GetOutput());


    tb3.GetOutput()->Update();

    PNMwriter writer;
    writer.SetInput(tb3.GetOutput());
    writer.Write(argv[2]);



    CheckSum cs;
    cs.SetInput(tb3.GetOutput());
    cs.OutputCheckSum("checksum");
}

void mirrors(char **argv){
	PNMreader reader(argv[1]);
    Shrinker sr;
    LRConcat lr1;
    TBConcat tb;
    Mirror mr;
    Rotate rt;
    Rotate rt2;

    sr.SetInput(reader.GetOutput());
    mr.SetInput(sr.GetOutput());
    lr1.SetInput(sr.GetOutput());
    lr1.SetInput2(mr.GetOutput());
    rt.SetInput(lr1.GetOutput());
    rt2.SetInput(rt.GetOutput());
    tb.SetInput(lr1.GetOutput());
    tb.SetInput2(rt2.GetOutput());
    tb.GetOutput()->Update();

    PNMwriter writer;
    writer.SetInput(tb.GetOutput());
    writer.Write(argv[2]);

    CheckSum cs;
    cs.SetInput(tb.GetOutput());
    cs.OutputCheckSum("checksum");
}

void blur(char **argv){
	PNMreader reader(argv[1]);
    Blur bl;

    bl.SetInput(reader.GetOutput());
    bl.GetOutput()->Update();

    PNMwriter writer;
    writer.SetInput(bl.GetOutput());
    writer.Write(argv[2]);

    CheckSum cs;
    cs.SetInput(bl.GetOutput());
    cs.OutputCheckSum("checksum");
}

void rotate(char **argv){
	PNMreader reader(argv[1]);
    Rotate rt;

    rt.SetInput(reader.GetOutput());
    rt.GetOutput()->Update();

    PNMwriter writer;
    writer.SetInput(rt.GetOutput());
    writer.Write(argv[2]);

    CheckSum cs;
    cs.SetInput(rt.GetOutput());
    cs.OutputCheckSum("checksum");
}

void grayscale(char **argv){
	PNMreader reader(argv[1]);
    Grayscale gs;

    gs.SetInput(reader.GetOutput());
    gs.GetOutput()->Update();

    PNMwriter writer;
    writer.SetInput(gs.GetOutput());
    writer.Write(argv[2]);

    CheckSum cs;
    cs.SetInput(gs.GetOutput());
    cs.OutputCheckSum("checksum");
}
void help()
{
	std::string stars = "************************************************************************";
	std::string filters = "The available filters are:\n\t1)330 (Doesn't work on every image, height and width must match)\n\t2)mirrors\n\t3)blur\n\t4)rotate\n\t5)grayscale\n";

	std::cout << stars << std::endl << filters << stars << std::endl;
}

int filters(char *s)
{
	if(strcmp(s, "330") == 0)
		return 1;
	else if(strcmp(s, "mirrors") == 0)
		return 2;
	else if(strcmp(s, "blur") == 0)
		return 3;
	else if(strcmp(s, "rotate") == 0)
		return 4;
	else if(strcmp(s, "grayscale") == 0)
		return 5;

	return -1;
}

int main(int argc, char *argv[])
{
    if(argc<2){
        std::cout << USAGE << std::endl;
        exit(0);
    }
    if(strcmp(argv[1], "--help") == 0){
    	help();
    	exit(0);
    }
    if(argc<4){
        std::cout << USAGE << std::endl;
        exit(0);
    }
    if(!outputCheck(argv[1])){
        std::cout << "input file needs to be of pnm format" << std::endl;
        exit(0);
    }
    if(!outputCheck(argv[2])){
        std::cout << "output file needs to be of pnm format" << std::endl;
        exit(0);
    }
    int fcase = filters(argv[3]);
    switch(fcase){
    	case 1:
    		_330(argv);
    		break;
    	case 2:
    		mirrors(argv);
    		break;
    	case 3:
    		blur(argv);
    		break;
    	case 4:
    		rotate(argv);
    		break;
    	case 5:
    		grayscale(argv);
    		break;
    	case -1:
    		help();
    		break;
    }	
}
