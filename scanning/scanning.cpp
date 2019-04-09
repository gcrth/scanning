#include "pch.h"
#include "tinyxml/tinyxml.h"
#include <iostream>
#include <string>
#include "pch.h"

enum TokenType
{
	keyword,
	identifier,
	constant,
	string_literal,
	punctuator
};

class token
{
public:
	int number;
	string value;
	int type;
	int line;
	bool valid;
	bool save(TiXmlElement* root)
	{
		char buf[1000];
		TiXmlElement* token = new TiXmlElement("token");
		root->LinkEndChild(token);

		TiXmlElement* attribute = new TiXmlElement("number");
		sprintf(buf,"%d",number);
		TiXmlText* text = new TiXmlText(buf);
		attribute->LinkEndChild(text);
		token->LinkEndChild(attribute);
		TiXmlElement* attribute = new TiXmlElement("value");
		TiXmlText* text = new TiXmlText(value.c_str());
		attribute->LinkEndChild(text);
		token->LinkEndChild(attribute);
		TiXmlElement* attribute = new TiXmlElement("type");
		switch (type)
		{
		case keyword:sprintf(buf, "keyword"); break;
		case identifier:sprintf(buf, "identifier"); break;
		case constant:sprintf(buf, "constant"); break;
		case string_literal:sprintf(buf, "string_literal"); break;
		case punctuator:sprintf(buf, "punctuator"); break;
		}
		TiXmlText* text = new TiXmlText(buf);
		attribute->LinkEndChild(text);
		token->LinkEndChild(attribute);
		TiXmlElement* attribute = new TiXmlElement("line");
		sprintf(buf, "%d", line);
		TiXmlText* text = new TiXmlText(buf);
		attribute->LinkEndChild(text);
		token->LinkEndChild(attribute);
		TiXmlElement* attribute = new TiXmlElement("valid");
		if(valid==true)		sprintf(buf, "true");
		else sprintf(buf, "false");
		TiXmlText* text = new TiXmlText(buf);
		attribute->LinkEndChild(text);
		token->LinkEndChild(attribute);
		return true;
	}
};

class inputBuffer
{
public:
	inputBuffer(string inFileName)
	{
		point = 0;
		const int linesize(1000);
		char lineBuf[linesize];
		if ((fp = fopen(inFileName.c_str(), "r")) == NULL) {
			printf("cannot open file/n"); exit(1);
		}
		while (!feof(fp)) {
			if (fgets(lineBuf, linesize, fp) != NULL)
			{
				buf += linesize;
			}
		}
		fclose(fp);
	}
	char read()
	{
		if(point<buf.size())return buf[point++];
		else return 0;
	}
	bool untread()
	{
		if (point > 0) 
		{
			point--; return true;
		}
		else
			return false;
	}
private:
	string buf;
	int point;
	FILE *fp;
};

class scanner
{

};

int main(int argc,char *argv[])
{
	if (argc != 3)return -1;
	string inFileName(argv[1]), outFileName(argv[2]);
	
	
}
