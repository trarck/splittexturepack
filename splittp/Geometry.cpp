#include "Geometry.h"


typedef std::vector<std::string> strArray;

static inline void split(std::string src, const char* token, strArray& vect)
{
    size_t nend=0;
    size_t nbegin=0;
    while(nend != -1)
    {
        nend = src.find(token, nbegin);
        if(nend == -1)
            vect.push_back(src.substr(nbegin, src.length()-nbegin));
        else
            vect.push_back(src.substr(nbegin, nend-nbegin));
        nbegin = nend + strlen(token);
    }
}

static bool splitWithForm(const char* pStr, strArray& strs)
{
    bool bRet = false;
    
    do
    {
        if(!pStr) break;
        
        // string is empty
        std::string content = pStr;
        if(content.length() == 0) break;
        
        size_t nPosLeft  = content.find('{');
        size_t nPosRight = content.find('}');
        
        // don't have '{' and '}'
        if(nPosLeft == (int)std::string::npos || nPosRight == (int)std::string::npos) break;
        // '}' is before '{'
        if(nPosLeft > nPosRight) break;
        
        std::string pointStr = content.substr(nPosLeft + 1, nPosRight - nPosLeft - 1);
        // nothing between '{' and '}'
        if(pointStr.length() == 0) break;
        
        size_t nPos1 = pointStr.find('{');
        size_t nPos2 = pointStr.find('}');
        // contain '{' or '}'
        if(nPos1 != (int)std::string::npos || nPos2 != (int)std::string::npos) break;
        
        split(pointStr, ",", strs);
        
        if (strs.size() != 2 || strs[0].length() == 0 || strs[1].length() == 0)
        {
            strs.clear();
            break;
        }
        
        bRet = true;
    } while (0);
    
    return bRet;
}

Rect RectFromString(const char* pszContent)
{
    Rect result = {0,0,0,0};
    
    do
    {
        std::string content = pszContent;
        
        // find the first '{' and the third '}'
        size_t nPosLeft  = content.find('{');
        size_t nPosRight = content.find('}');
        for (int i = 1; i < 3; ++i)
        {
            if (nPosRight == (int)std::string::npos)
            {
                break;
            }
            nPosRight = content.find('}', nPosRight + 1);
        }
        if (nPosLeft == (int)std::string::npos || nPosRight == (int)std::string::npos){
            break;
        }
        
        content = content.substr(nPosLeft + 1, nPosRight - nPosLeft - 1);
        size_t nPointEnd = content.find('}');
        if(nPointEnd == (int)std::string::npos){
            break;
        }
        nPointEnd = content.find(',', nPointEnd);
        if(nPointEnd == (int)std::string::npos){
            break;
        }
        
        // get the point string and size string
        std::string pointStr = content.substr(0, nPointEnd);
        std::string sizeStr  = content.substr(nPointEnd + 1, content.length() - nPointEnd);
        
        // split the string with ','
        strArray pointInfo;
        if(!splitWithForm(pointStr.c_str(), pointInfo)) break;
        strArray sizeInfo;
        if(!splitWithForm(sizeStr.c_str(), sizeInfo)) break;
        
        float x = (float) atof(pointInfo[0].c_str());
        float y = (float) atof(pointInfo[1].c_str());
        float width  = (float) atof(sizeInfo[0].c_str());
        float height = (float) atof(sizeInfo[1].c_str());
        
        result.x=x;
        result.y=y;
        result.width=width;
        result.height=height;
        
    } while (0);
    
    return result;
}

Point PointFromString(const char* pszContent)
{
    Point ret = {0,0};
    
    do
    {
        strArray strs;
        if(!splitWithForm(pszContent, strs)) break;
        
        float x = (float) atof(strs[0].c_str());
        float y = (float) atof(strs[1].c_str());
        
        ret.x=x;
        ret.y=y;
        
    } while (0);
    
    return ret;
}

Size SizeFromString(const char* pszContent)
{
    Size ret = {0,0};
    
    do
    {
        strArray strs;
        if(!splitWithForm(pszContent, strs)) break;
        
        float width  = (float) atof(strs[0].c_str());
        float height = (float) atof(strs[1].c_str());
        
        ret.width=width;
        ret.height=height;
        
    } while (0);
    
    return ret;
}
