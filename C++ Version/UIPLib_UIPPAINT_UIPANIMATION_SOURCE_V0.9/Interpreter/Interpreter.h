#ifndef __INTERPRETER_H
#define __INTERPRETER_H

#include "Proxy_count_ref.h"

class CPlayContext : virtual public CRefCounter 
{  
public:  
    string GetPlayText()  
    {  
        return m_strText;  
    }  
    void SetPlayText(const string& strText)  
    {  
        m_strText = strText;  
    }  

private:  
    string m_strText;  
};  

struct ICommand : virtual public CRefCounter
{
	 virtual void Excute(string& strKey, int nValue) = 0;  
};


class CExpression  : public ICommand,virtual public CRefCounter
{  
public:  
    // ������  
    void Interpret(CPlayContext& playContext)  
    {  
        if (playContext.GetPlayText().empty())  
        {  
            return;  
        }  
        else  
        {  
            string strPlayKey = playContext.GetPlayText().substr(0, 1);  
            string strtemp = playContext.GetPlayText().substr(2);  
            playContext.SetPlayText(strtemp);  
              
            size_t nPos = playContext.GetPlayText().find(" ");  
            string strPlayValue = playContext.GetPlayText().substr(0, nPos);  
            int    nPlayValue = atoi(strPlayValue.c_str());  
            nPos = playContext.GetPlayText().find(" ");  
            playContext.SetPlayText(playContext.GetPlayText().substr(nPos + 1));  
            Excute(strPlayKey, nPlayValue);  
        }  
    }  
};  

typedef CSmartPointer<CExpression> CExpPointer;

//������
class CNote : public CExpression  
{  
public:  
    virtual void Excute(string& strKey, int nValue)  
    {  
        char szKey[2];  
        strncpy(szKey, strKey.c_str(), strKey.length());  
        string strNote;  
        switch (szKey[0])  
        {  
        case 'C':  
            strNote = "1";  
            break;  
        case 'D':  
            strNote = "2";  
            break;  
        case 'E':  
            strNote = "3";  
            break;  
        case 'F':  
            strNote = "4";  
            break;  
        case 'G':  
            strNote = "5";  
            break;  
        case 'A':  
            strNote = "6";  
            break;  
        case 'B':  
            strNote = "7";  
            break;  
        default:  
            strNote = "error";  
            break;  
        }  
		printf("%s ",strNote.c_str());  
    }  
};  

//������
class CScale : public CExpression  
{  
public:  
    virtual void Excute(string& strKey, int nValue)  
    {  
        string strScale;  
        switch (nValue)  
        {  
        case 1:  
            strScale = "����";  
            break;  
        case 2:  
            strScale = "����";  
            break;  
        case 3:  
            strScale = "����";  
            break;  
        default:  
            strScale = "error";  
            break;  
        }  
		printf("%s ",strScale.c_str());  
    }   
};  


//������
class CSpeed : public CExpression  
{  
public:  
    virtual void Excute(string& strKey, int nValue)  
    {  
        string strSpeed;  
        if (nValue < 3)  
        {  
            strSpeed = "����";  
        }  
        else if (nValue >= 6)  
        {  
            strSpeed = "����";  
        }  
        else  
        {  
            strSpeed = "����";  
        }  
		printf("%s ",strSpeed.c_str()) ;  
    }  
};  


void ClientTest()
{
   CPlayContext context;  
   printf("Music:");  
      
    context.SetPlayText("T 2 O 2 E  G  G ");  
    CExpPointer expressObj = NULL;  
      
    while (!context.GetPlayText().empty())  
    {  
        string strSep = context.GetPlayText().substr(0, 1);  
        char szKey[2];  
        strncpy(szKey, strSep.c_str(), strSep.length());  
        switch (szKey[0])  
        {  
        case 'O':  
            expressObj = new CScale();  
            break;  
        case 'T':  
            expressObj = new CSpeed();  
            break;  
        case 'C':  
        case 'D':  
        case 'E':  
        case 'F':  
        case 'G':  
        case 'A':  
        case 'B':    
            expressObj = new CNote();  
            break;  
        default:  
            break;  
        }  
        if (NULL != expressObj)  
        {  
            expressObj->Interpret(context);  
        }  
    }  
}


#endif