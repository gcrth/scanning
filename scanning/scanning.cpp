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

enum State
{
	start,
	identifierMid,

};

class Token
{
public:
	Token()
	{
		valid = true;
	}
	Token(int number_, string value_, int type_, int line_, TiXmlElement* root, bool valid_ = true)
	{
		 number= number_;
		 value= value_;
		 type= type_;
		 line= line_;
		 valid= valid_;
		 save(root);
	}
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

		TiXmlElement* attribute;
		TiXmlText* text;

		attribute = new TiXmlElement("number");
		sprintf(buf,"%d",number);
		text = new TiXmlText(buf);
		attribute->LinkEndChild(text);
		token->LinkEndChild(attribute);
		attribute = new TiXmlElement("value");
		text = new TiXmlText(value.c_str());
		attribute->LinkEndChild(text);
		token->LinkEndChild(attribute);
		attribute = new TiXmlElement("type");
		switch (type)
		{
		case keyword:sprintf(buf, "keyword"); break;
		case identifier:sprintf(buf, "identifier"); break;
		case constant:sprintf(buf, "constant"); break;
		case string_literal:sprintf(buf, "string_literal"); break;
		case punctuator:sprintf(buf, "punctuator"); break;
		}
		text = new TiXmlText(buf);
		attribute->LinkEndChild(text);
		token->LinkEndChild(attribute);
		attribute = new TiXmlElement("line");
		sprintf(buf, "%d", line);
		text = new TiXmlText(buf);
		attribute->LinkEndChild(text);
		token->LinkEndChild(attribute);
		attribute = new TiXmlElement("valid");
		if(valid==true)		sprintf(buf, "true");
		else sprintf(buf, "false");
		text = new TiXmlText(buf);
		attribute->LinkEndChild(text);
		token->LinkEndChild(attribute);
		return true;
	}
};

class InputBuffer
{
public:
	InputBuffer(string inFileName)
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
				buf += lineBuf;
			}
		}
		fclose(fp);
	}
	char read()
	{
		if(point<(int)buf.size())return buf[point++];
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
public:
	scanner(string inFileName,string outFileName)
	{
		saveName= outFileName;
		BufName= inFileName;
	}
	bool run()
	{
		InputBuffer buf(BufName);
		TiXmlDeclaration* dec = new TiXmlDeclaration("1.0", "UTF-8", "");
		doc.LinkEndChild(dec);
		TiXmlElement * project = new TiXmlElement("project");
		project->SetAttribute("name", BufName.c_str());
		doc.LinkEndChild(project);
		tokens = new TiXmlElement("tokens");
		project->LinkEndChild(tokens);

		int line(1);
		int number(1);
		string tokenValue;
		int state(0);

		while (1)
		{
			char charToProcess=buf.read();
			switch (state)
			{
			case 0:
				switch (charToProcess)
				{
				case '[': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
				case ']': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
				case '(': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
				case ')': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
				case '{': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
				case '}': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
				case '?': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
				case ':': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
				case ';': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
				case ',': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
				case '~': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }


				}

			//case '':
			case 51: switch (charToProcess)
			{
			case '>': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
			case '-': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
			case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();number++; break; }
			default: {buf.untread(); link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			}//- -> -- -=
			case 52: switch (charToProcess)
			{
			case '+': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			default: {buf.untread(); link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			}//+ += ++
			case 53: switch (charToProcess)
			{
			case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			default: {buf.untread(); link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			}//= ==
			case 54: switch (charToProcess)
			{
			case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			default: {buf.untread(); link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			}//* *=
			case 55: switch (charToProcess)
			{
			case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			default: {buf.untread(); link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			}/// /=
			case 56: switch (charToProcess)
			{
			case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			default: {buf.untread(); link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			}//% %=
			case 57: switch (charToProcess)
			{
			case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			default: {buf.untread(); link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			}//<< <<=
			case 58: switch (charToProcess)
			{
			case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			default: {buf.untread(); link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			}//>> >>=
			case 59: switch (charToProcess)
			{

			}//& &= &&
			case 60: switch (charToProcess)
			{
			case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			default: {buf.untread(); link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
			}//^ ^=
			case 61: switch (charToProcess)
			{

			}//| |= ||
			case 62: switch (charToProcess)
			{

			}//< <=
			case 63: switch (charToProcess)
			{

			}//> >=
			case 64: switch (charToProcess)
			{

			}//! !=



			}
		}
	}
	bool save()
	{

	}
	bool isKeyWord(string x)
	{
		if (x == "auto")return true;
		else if (x == "break")return true;
		else if (x == "case")return true;
		else if (x == "char")return true;
		else if (x == "const")return true;
		else if (x == "continue")return true;
		else if (x == "default")return true;
		else if (x == "do")return true;
		else if (x == "double")return true;
		else if (x == "else")return true;
		else if (x == "enum")return true;
		else if (x == "extern")return true;
		else if (x == "float")return true;
		else if (x == "for")return true;
		else if (x == "goto")return true;
		else if (x == "if")return true;
		else if (x == "inline")return true;
		else if (x == "int")return true;
		else if (x == "long")return true;
		else if (x == "register")return true;
		else if (x == "restrict")return true;
		else if (x == "return")return true;
		else if (x == "short")return true;
		else if (x == "signed")return true;
		else if (x == "sizeof")return true;
		else if (x == "static")return true;
		else if (x == "struct")return true;
		else if (x == "switch")return true;
		else if (x == "typedef")return true;
		else if (x == "union")return true;
		else if (x == "unsigned")return true;
		else if (x == "void")return true;
		else if (x == "volatile")return true;
		else if (x == "while")return true;
		else if (x == "_Alignas")return true;
		else if (x == "_Alignof")return true;
		else if (x == "_Atomic")return true;
		else if (x == "_Bool")return true;
		else if (x == "_Complex")return true;
		else if (x == "_Generic")return true;
		else if (x == "_Imaginary")return true;
		else if (x == "_Noreturn")return true;
		else if (x == "_Static_assert")return true;
		else if (x == "_Thread_local")return true;
		else return false;
	}
	bool link(int number_, string value_, int type_, int line_, TiXmlElement* root, bool valid_ = true)
	{
		Token token(number_, value_, type_, line_, root, valid_);
		return true;
	}


private:
	TiXmlDocument doc;
	string saveName;
	string BufName;
	TiXmlElement * tokens;
};

int main(int argc,char *argv[])
{
	if (argc != 3)return -1;
	string inFileName(argv[1]), outFileName(argv[2]);
	
	
}
