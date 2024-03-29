#include "pch.h"
#include "tinyxml.h"
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

class Token
{
  public:
	Token()
	{
		valid = true;
	}
	Token(int number_, string value_, int type_, int line_, TiXmlElement *root, bool valid_ = true)
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
	bool save(TiXmlElement *root)
	{
		char buf[1000];
		TiXmlElement *token = new TiXmlElement("token");
		root->LinkEndChild(token);

		TiXmlElement *attribute;
		TiXmlText *text;

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
		case keyword:
			sprintf(buf, "keyword");
			break;
		case identifier:
			sprintf(buf, "identifier");
			break;
		case constant:
			sprintf(buf, "constant");
			break;
		case string_literal:
			sprintf(buf, "string_literal");
			break;
		case punctuator:
			sprintf(buf, "punctuator");
			break;
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
		if (valid == true)
			sprintf(buf, "true");
		else
			sprintf(buf, "false");
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
		if ((fp = fopen(inFileName.c_str(), "r")) == NULL)
		{
			printf("cannot open file/n");
			exit(1);
		}
		while (!feof(fp))
		{
			if (fgets(lineBuf, linesize, fp) != NULL)
			{
				buf += lineBuf;
			}
		}
		fclose(fp);
	}
	char read()
	{
		if (point < (int)buf.size())
			return buf[point++];
		else
			return 0;
	}
	bool untread()
	{
		if (point > 0)
		{
			point--;
			return true;
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
		TiXmlDeclaration *dec = new TiXmlDeclaration("1.0", "UTF-8", "");
		doc.LinkEndChild(dec);
		TiXmlElement *project = new TiXmlElement("project");
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
				case 0:
				{
					isEnd = true;
					break;
				}
				case '[':
				{
					tokenValue = charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					number++;
					break;
				}
				case ']':
				{
					tokenValue = charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					number++;
					break;
				}
				case '(':
				{
					tokenValue = charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					number++;
					break;
				}
				case ')':
				{
					tokenValue = charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					number++;
					break;
				}
				case '{':
				{
					tokenValue = charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					number++;
					break;
				}
				case '}':
				{
					tokenValue = charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					number++;
					break;
				}
				case '?':
				{
					tokenValue = charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					number++;
					break;
				}
				case ';':
				{
					tokenValue = charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					number++;
					break;
				}
				case ',':
				{
					tokenValue = charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					number++;
					break;
				}
				case '~':
				{
					tokenValue = charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					number++;
					break;
				}

				case '+':
				{
					state = 51;
					tokenValue += charToProcess;
					break;
				}
				case '&':
				{
					state = 52;
					tokenValue += charToProcess;
					break;
				}
				case '|':
				{
					state = 53;
					tokenValue += charToProcess;
					break;
				}
				case '<':
				{
					state = 54;
					tokenValue += charToProcess;
					break;
				}
				case '>':
				{
					state = 55;
					tokenValue += charToProcess;
					break;
				}
				case '-':
				{
					state = 56;
					tokenValue += charToProcess;
					break;
				}
				case ':':
				{
					state = 59;
					tokenValue += charToProcess;
					break;
				}
				case '%':
				{
					state = 60;
					tokenValue += charToProcess;
					break;
				}
				case '!':
				case '=':
				case '*':
				case '/':
				case '^':
				{
					state = 58;
					tokenValue += charToProcess;
					break;
				}

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
				case 'M':
				case 'N':
				case 'O':
				case 'P':
				case 'Q':
				case 'R':
				case 'S':
				case 'T':
				case 'V':
				case 'W':
				case 'X':
				case 'Y':
				case 'Z':
				{
					state = 1;
					tokenValue += charToProcess;
					break;
				}

				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					state = 3;
					tokenValue += charToProcess;
					break;
				}

				case '0':
				{
					state = 4;
					tokenValue += charToProcess;
					break;
				}

				case '.':
				{
					state = 18;
					tokenValue += charToProcess;
					break;
				}

				case 'L':
				case 'U':
				{
					state = 32;
					tokenValue += charToProcess;
					break;
				}

				case 'u':
				{
					state = 33;
					tokenValue += charToProcess;
					break;
				}

				case '\"':
				{
					state = 35;
					tokenValue += charToProcess;
					break;
				}
				case '\'':
				{
					state = 28;
					tokenValue += charToProcess;
					break;
				}

				case '\n':
				{
					line++;
					break;
				}
				case '\t':
				case ' ':
				{
					break;
				}

				default:
				{
					reportError(number++, line);
					break;
				}
				}
				break;

				break;
			case 1:
				{
					switch (charToProcess)
					{
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
					case 'M':
					case 'N':
					case 'O':
					case 'P':
					case 'Q':
					case 'R':
					case 'S':
					case 'T':
					case 'V':
					case 'W':
					case 'X':
					case 'Y':
					case 'Z':
					case 'L':
					case 'u':
					case 'U':
					case '_':
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
					{
						state = 1;
						tokenValue += charToProcess;
						break;
					}
					default:
					{
						buf.untread();
						if (isKeyWord(tokenValue))
							link(number, tokenValue, keyword, line, tokens, true);
						else
							link(number, tokenValue, identifier, line, tokens, true);
						tokenValue.clear();
						state = 0;
						number++;
						break;
					}
					}
				}
				break;
			case 3:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					state = 3;
					tokenValue += charToProcess;
					break;
				}

				case 'l':
				{
					state = 10;
					tokenValue += charToProcess;
					break;
				}
				case 'L':
				{
					state = 11;
					tokenValue += charToProcess;
					break;
				}
				case 'u':
				case 'U':
				{
					state = 9;
					tokenValue += charToProcess;
					break;
				}

				case '.':
				{
					state = 15;
					tokenValue += charToProcess;
					break;
				}
				case 'e':
				case 'E':
				{
					state = 16;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 4:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				{
					state = 7;
					tokenValue += charToProcess;
					break;
				}

				case '8':
				case '9':
				{
					state = 27;
					tokenValue += charToProcess;
					break;
				}

				case 'x':
				case 'X':
				{
					state = 8;
					tokenValue += charToProcess;
					break;
				}

				case 'l':
				{
					state = 10;
					tokenValue += charToProcess;
					break;
				}
				case 'L':
				{
					state = 11;
					tokenValue += charToProcess;
					break;
				}
				case 'u':
				case 'U':
				{
					state = 9;
					tokenValue += charToProcess;
					break;
				}

				case '.':
				{
					state = 15;
					tokenValue += charToProcess;
					break;
				}
				case 'e':
				case 'E':
				{
					state = 16;
					tokenValue += charToProcess;
					break;
				}

				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 5:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				{
					state = 5;
					tokenValue += charToProcess;
					break;
				}

				case 'l':
				{
					state = 10;
					tokenValue += charToProcess;
					break;
				}
				case 'L':
				{
					state = 11;
					tokenValue += charToProcess;
					break;
				}
				case 'u':
				case 'U':
				{
					state = 9;
					tokenValue += charToProcess;
					break;
				}

				case '.':
				{
					state = 20;
					tokenValue += charToProcess;
					break;
				}
				case 'p':
				case 'P':
				{
					state = 24;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 7:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				{
					state = 7;
					tokenValue += charToProcess;
					break;
				}

				case '8':
				case '9':
				{
					state = 27;
					tokenValue += charToProcess;
					break;
				}

				case 'l':
				{
					state = 10;
					tokenValue += charToProcess;
					break;
				}
				case 'L':
				{
					state = 11;
					tokenValue += charToProcess;
					break;
				}
				case 'u':
				case 'U':
				{
					state = 9;
					tokenValue += charToProcess;
					break;
				}

				case '.':
				{
					state = 15;
					tokenValue += charToProcess;
					break;
				}
				case 'e':
				case 'E':
				{
					state = 16;
					tokenValue += charToProcess;
					break;
				}

				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 8:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				{
					state = 5;
					tokenValue += charToProcess;
					break;
				}

				case '.':
				{
					state = 21;
					tokenValue += charToProcess;
					break;
				}

				default:
				{
					reportError(number++, line);
					break;
				}
				}
				break;
			case 9:
				switch (charToProcess)
				{
				case 'l':
				{
					state = 40;
					tokenValue += charToProcess;
					break;
				}
				case 'L':
				{
					state = 42;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 10:
				switch (charToProcess)
				{
				case 'u':
				case 'U':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				case 'l':
				{
					state = 12;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 11:
				switch (charToProcess)
				{
				case 'u':
				case 'U':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				case 'L':
				{
					state = 12;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 12:
				switch (charToProcess)
				{
				case 'u':
				case 'U':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 15:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					state = 15;
					tokenValue += charToProcess;
					break;
				}

				case 'e':
				case 'E':
				{
					state = 16;
					tokenValue += charToProcess;
					break;
				}

				case 'f':
				case 'F':
				case 'l':
				case 'L':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 16:
				switch (charToProcess)
				{
				case '+':
				case '-':
				{
					state = 17;
					tokenValue += charToProcess;
					break;
				}

				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					state = 19;
					tokenValue += charToProcess;
					break;
				}

				default:
				{
					reportError(number++, line);
					break;
				}
				}
				break;
			case 17:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					state = 19;
					tokenValue += charToProcess;
					break;
				}

				default:
				{
					reportError(number++, line);
					break;
				}
				}
				break;
			case 18:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					state = 15;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					buf.untread();
					state = 57;
					break;
				}
				}
				break;
			case 19:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					state = 19;
					tokenValue += charToProcess;
					break;
				}

				case 'f':
				case 'F':
				case 'l':
				case 'L':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 20:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				{
					state = 20;
					tokenValue += charToProcess;
					break;
				}

				case 'p':
				case 'P':
				{
					state = 24;
					tokenValue += charToProcess;
					break;
				}

				default:
				{
					reportError(number++, line);
					break;
				}
				}
				break;
			case 21:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				{
					state = 20;
					tokenValue += charToProcess;
					break;
				}

				default:
				{
					reportError(number++, line);
					break;
				}
				}
				break;
			case 22:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					state = 22;
					tokenValue += charToProcess;
					break;
				}

				case 'f':
				case 'F':
				case 'l':
				case 'L':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 23:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					state = 22;
					tokenValue += charToProcess;
					break;
				}

				default:
				{
					reportError(number++, line);
					break;
				}
				}
				break;
			case 24:
				switch (charToProcess)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					state = 22;
					tokenValue += charToProcess;
					break;
				}

				case '+':
				case '-':
				{
					state = 23;
					tokenValue += charToProcess;
					break;
				}

				default:
				{
					reportError(number++, line);
					break;
				}
				}
				break;
			case 27:
				switch (charToProcess)
				{
				case '.':
				{
					state = 15;
					tokenValue += charToProcess;
					break;
				}
				case 'e':
				case 'E':
				{
					state = 16;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					reportError(number++, line);
					break;
				}
				}
				break;
			case 28:
				switch (charToProcess)
				{
				case '\\':
				{
					state = 31;
					tokenValue += charToProcess;
					break;
				}
				case '\'':
				case '\r':
				case '\n':
				{
					reportError(number++, line);
					break;
				}
				default:
				{
					state = 29;
					tokenValue += charToProcess;
					break;
				}
				}
				break;
			case 29:
				switch (charToProcess)
				{
				case '\\':
				{
					state = 31;
					tokenValue += charToProcess;
					break;
				}
				case '\'':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				case '\r':
				case '\n':
				{
					reportError(number++, line);
					break;
				}
				default:
				{
					state = 29;
					tokenValue += charToProcess;
					break;
				}
				}
				break;
			case 31:
				switch (charToProcess)
				{
				case '\'':
				case '\"':
				case '?':
				case '\\':
				case 'a':
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
				case 'v':
				{
					state = 29;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					reportError(number++, line);
					break;
				}
				}
				break;
			case 32:
				switch (charToProcess)
				{
				case '\'':
				{
					state = 28;
					tokenValue += charToProcess;
					break;
				}
				case '\"':
				{
					state = 35;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					state = 1;
					buf.untread();
					break;
				}
				}
				break;
			case 33:
				switch (charToProcess)
				{
				case '\'':
				{
					state = 28;
					tokenValue += charToProcess;
					break;
				}
				case '\"':
				{
					state = 35;
					tokenValue += charToProcess;
					break;
				}
				case '8':
				{
					state = 34;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					state = 1;
					buf.untread();
					break;
				}
				}
				break;
			case 34:
				switch (charToProcess)
				{
				case '\"':
				{
					state = 35;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					state = 1;
					buf.untread();
					break;
				}
				}
				break;
			case 35:
				switch (charToProcess)
				{
				case '\\':
				{
					state = 37;
					tokenValue += charToProcess;
					break;
				}
				case '\"':
				case '\r':
				case '\n':
				{
					reportError(number++, line);
					break;
				}
				default:
				{
					state = 36;
					tokenValue += charToProcess;
					break;
				}
				}
				break;
			case 36:
				switch (charToProcess)
				{
				case '\\':
				{
					state = 37;
					tokenValue += charToProcess;
					break;
				}
				case '\"':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, string_literal, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				case '\r':
				case '\n':
				{
					reportError(number++, line);
					break;
				}
				default:
				{
					state = 36;
					tokenValue += charToProcess;
					break;
				}
				}
				break;
			case 37:
				switch (charToProcess)
				{
				case '\'':
				case '\"':
				case '?':
				case '\\':
				case 'a':
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
				case 'v':
				{
					state = 36;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					reportError(number++, line);
					break;
				}
				}
				break;
			case 40:
				switch (charToProcess)
				{
				case 'l':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 42:
				switch (charToProcess)
				{
				case 'L':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, constant, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				}
				break;
			case 51:
				switch (charToProcess)
				{
				case '+':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				case '=':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				} //+ += ++
				break;
			case 52:
				switch (charToProcess)
				{
				case '&':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				case '=':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				} //& &= &&
				break;
			case 53:
				switch (charToProcess)
				{
				case '|':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				case '=':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				} //| |= ||
				break;
			case 54:
				switch (charToProcess)
				{
				case ':':
				case '%':
				case '=':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				case '<':
				{
					state = 58;
					tokenValue += charToProcess;
					break;
				}

				default:
				{
					buf.untread();
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				} //< <= << <<= <: <%
				break;
			case 55:
				switch (charToProcess)
				{
				case '=':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				case '>':
				{
					state = 58;
					tokenValue += charToProcess;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				} //> >= >> >>=
				break;
			case 56:
				switch (charToProcess)
				{
				case '>':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				case '-':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				case '=':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				} //- -> -- -=
				break;
			case 57:
				switch (charToProcess)
				{
				case '.':
				{
					if (buf.read() == '.')
					{
						tokenValue = "...";
						link(number, tokenValue, punctuator, line, tokens, true);
						tokenValue.clear();
						state = 0;
						number++;
						break;
					}
					else
						reportError(number++, line);
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				} //. ...
				break;
			case 58:
				switch (charToProcess)
				{
				case '=':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				} // ! != = == * *= / /= ^ ^=
				break;
			case 59:
				switch (charToProcess)
				{
				case '>':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				} // :> %>
				break;
			case 60:
				switch (charToProcess)
				{
				case '=':
				case '>':
				{
					tokenValue += charToProcess;
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				default:
				{
					buf.untread();
					link(number, tokenValue, punctuator, line, tokens, true);
					tokenValue.clear();
					state = 0;
					number++;
					break;
				}
				} // %= %>
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
		if (x == "auto")
			return true;
		else if (x == "break")
			return true;
		else if (x == "case")
			return true;
		else if (x == "char")
			return true;
		else if (x == "const")
			return true;
		else if (x == "continue")
			return true;
		else if (x == "default")
			return true;
		else if (x == "do")
			return true;
		else if (x == "double")
			return true;
		else if (x == "else")
			return true;
		else if (x == "enum")
			return true;
		else if (x == "extern")
			return true;
		else if (x == "float")
			return true;
		else if (x == "for")
			return true;
		else if (x == "goto")
			return true;
		else if (x == "if")
			return true;
		else if (x == "inline")
			return true;
		else if (x == "int")
			return true;
		else if (x == "long")
			return true;
		else if (x == "register")
			return true;
		else if (x == "restrict")
			return true;
		else if (x == "return")
			return true;
		else if (x == "short")
			return true;
		else if (x == "signed")
			return true;
		else if (x == "sizeof")
			return true;
		else if (x == "static")
			return true;
		else if (x == "struct")
			return true;
		else if (x == "switch")
			return true;
		else if (x == "typedef")
			return true;
		else if (x == "union")
			return true;
		else if (x == "unsigned")
			return true;
		else if (x == "void")
			return true;
		else if (x == "volatile")
			return true;
		else if (x == "while")
			return true;
		else if (x == "_Alignas")
			return true;
		else if (x == "_Alignof")
			return true;
		else if (x == "_Atomic")
			return true;
		else if (x == "_Bool")
			return true;
		else if (x == "_Complex")
			return true;
		else if (x == "_Generic")
			return true;
		else if (x == "_Imaginary")
			return true;
		else if (x == "_Noreturn")
			return true;
		else if (x == "_Static_assert")
			return true;
		else if (x == "_Thread_local")
			return true;
		else
			return false;
	}
	bool link(int number_, string value_, int type_, int line_, TiXmlElement *root, bool valid_ = true)
	{
		Token token(number_, value_, type_, line_, root, valid_);
		return true;
	}
	void reportError(int number, int line)
	{
		cout << "error at token " << number << " line " << line << "\n";
		exit(-1);
	}

  private:
	TiXmlDocument doc;
	string saveName;
	string BufName;
	TiXmlElement *tokens;
};

int main(int argc, char *argv[])
{
	if (argc != 3)
		return -1;
	string inFileName(argv[1]), outFileName(argv[2]);
	Scanner scannner(inFileName, outFileName);
	cout << "end\n";
	return 0;
}
