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
		number = number_;
		value = value_;
		type = type_;
		line = line_;
		valid = valid_;
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
		sprintf(buf, "%d", number);
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
		if (valid == true)		sprintf(buf, "true");
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
		if (point < (int)buf.size())return buf[point++];
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

class Scanner
{
public:
	Scanner(string inFileName, string outFileName)
	{
		saveName = outFileName;
		BufName = inFileName;
		run();
		save();
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
		bool isEnd = false;

		while (!isEnd)
		{
			char charToProcess = buf.read();
			switch (state)
			{
			case 0:
				switch (charToProcess)
				{
				case 0: {isEnd = true; break; }
				case '[': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
				case ']': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
				case '(': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
				case ')': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
				case '{': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
				case '}': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
				case '?': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
				case ':': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
				case ';': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
				case ',': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }
				case '~': {tokenValue = charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear(); number++; break; }

				case '+': {state = 51; tokenValue += charToProcess; break; }
				case '&': {state = 52; tokenValue += charToProcess; break; }
				case '|': {state = 53; tokenValue += charToProcess; break; }
				case '<': {state = 54; tokenValue += charToProcess; break; }
				case '>': {state = 55; tokenValue += charToProcess; break; }
				case '-': {state = 56; tokenValue += charToProcess; break; }
				case '!':
				case '=':
				case '*':
				case '/':
				case '%':
				case '^': {state = 58; tokenValue += charToProcess; break; }

				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'g':
				case 'h':
				case 'i':
				case 'j':
				case 'k':
				case 'l':
				case 'm':
				case 'n':
				case 'o':
				case 'p':
				case 'q':
				case 'r':
				case 's':
				case 't':
				case 'u':
				case 'v':
				case 'w':
				case 'x':
				case 'y':
				case 'z':

				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				case 'G':
				case 'H':
				case 'I':
				case 'J':
				case 'K':
				case 'L':
				case 'M':
				case 'N':
				case 'O':
				case 'P':
				case 'Q':
				case 'R':
				case 'S':
				case 'T':
				case 'U':
				case 'V':
				case 'W':
				case 'X':
				case 'Y':
				case 'Z':
					break;



				case '\n': {line++; break; }
				case '\t':
				case ' ': {break; }

				default: {reportError(number++, line); break; }

				}break;

				break;
			case 51:
				switch (charToProcess)
				{
				case '+': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				default: {buf.untread(); state=0;  link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				}//+ += ++
				break;
			case 52:
				switch (charToProcess)
				{
				case '&': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				default: {buf.untread(); state=0; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				}//& &= &&
				break;
			case 53:
				switch (charToProcess)
				{
				case '|': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				default: {buf.untread(); state=0; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				}//| |= ||
				break;
			case 54:
				switch (charToProcess)
				{
				case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				case '<': {state = 58; tokenValue += charToProcess; break; }
				default: {buf.untread(); state=0; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				}//< <= << <<= 
				break;
			case 55:
				switch (charToProcess)
				{
				case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				case '>': {state = 58; tokenValue += charToProcess; break; }
				default: {buf.untread(); state=0; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				}//> >= >> >>= 
				break;
			case 56:
				switch (charToProcess)
				{
				case '>': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				case '-': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				default: {buf.untread(); state=0; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				}//- -> -- -=
				break;
			case 57:
				switch (charToProcess)
				{
				case '.':
				{
					if (buf.read() == '.') { tokenValue = "..."; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
					else reportError(number++, line);
				}
				default: {buf.untread(); state=0; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				}//. ...
				break;
			case 58:
				switch (charToProcess)
				{
				case '=': {tokenValue += charToProcess; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				default: {buf.untread(); state=0; link(number, tokenValue, punctuator, line, tokens, true); tokenValue.clear();state=0; number++; break; }
				}// ! != = == * *= / /= % %= ^ ^=
				break;





			}
		}
		return true;
	}
	bool save()
	{
		doc.SaveFile(saveName.c_str());
		return true;
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
	void reportError(int number, int line)
	{
		cout << "error at" << number << "token" << line << "line\n";
		exit(-1);
	}

private:
	TiXmlDocument doc;
	string saveName;
	string BufName;
	TiXmlElement * tokens;
};

int main(int argc, char *argv[])
{
	if (argc != 3)return -1;
	string inFileName(argv[1]), outFileName(argv[2]);
	Scanner scannner(inFileName, outFileName);
	cout << "end\n";
	return 0;
}
