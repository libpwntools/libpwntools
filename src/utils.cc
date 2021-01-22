#include <string>
#include "utils.h"
#include <ostream>

bool ends_with(const std::string& a, const std::string& b) {
    if (b.size() > a.size()) return false;
    return std::equal(a.begin() + a.size() - b.size(), a.end(), b.begin());
}

template<class Elem, class Traits>
        inline void hex_dump(const void* aData, std::size_t aLength, std::basic_ostream<Elem, Traits>& aStream, std::size_t aWidth = 16)
        {
                const char* const start = static_cast<const char*>(aData);
                const char* const end = start + aLength;
                const char* line = start;
                while (line != end)
                {
                        aStream.width(4);
                        aStream.fill('0');
                        aStream << std::hex << line - start << " : ";
                        std::size_t lineLength = std::min(aWidth, static_cast<std::size_t>(end - line));
                        for (std::size_t pass = 1; pass <= 2; ++pass)
                        {
                                for (const char* next = line; next != end && next != line + aWidth; ++next)
                                {
                                        char ch = *next;
                                        switch(pass)
                                        {
                                        case 1:
                                                aStream << (ch < 32 ? '.' : ch);
                                                break;
                                        case 2:
                                                if (next != line)
                                                        aStream << " ";
                                                aStream.width(2);
                                                aStream.fill('0');
                                                aStream << std::hex << std::uppercase << static_cast<int>(static_cast<unsigned char>(ch));
                                                break;
                                        }
                                }
                                if (pass == 1 && lineLength != aWidth)
                                        aStream << std::string(aWidth - lineLength, ' ');
                                aStream << " ";
                        }
                        aStream << std::endl;
                        line = line + lineLength;
                }
}

void hexdump(void *pAddressIn, long  lSize)
{
 char szBuf[100];
 long lIndent = 1;
 long lOutLen, lIndex, lIndex2, lOutLen2;
 long lRelPos;
 struct { char *pData; unsigned long lSize; } buf;
 unsigned char *pTmp,ucTmp;
 unsigned char *pAddress = (unsigned char *)pAddressIn;

   buf.pData   = (char *)pAddress;
   buf.lSize   = lSize;

   while (buf.lSize > 0)
   {
      pTmp     = (unsigned char *)buf.pData;
      lOutLen  = (int)buf.lSize;
      if (lOutLen > 16)
          lOutLen = 16;

      // create a 64-character formatted output line:
      sprintf(szBuf, " >                            "
                     "                      "
                     "    %08lX", pTmp-pAddress);
      lOutLen2 = lOutLen;

      for(lIndex = 1+lIndent, lIndex2 = 53-15+lIndent, lRelPos = 0;
          lOutLen2;
          lOutLen2--, lIndex += 2, lIndex2++
         )
      {
         ucTmp = *pTmp++;

         sprintf(szBuf + lIndex, "%02X ", (unsigned short)ucTmp);
         if(!isprint(ucTmp))  ucTmp = '.'; // nonprintable char
         szBuf[lIndex2] = ucTmp;

         if (!(++lRelPos & 3))     // extra blank after 4 bytes
         {  lIndex++; szBuf[lIndex+2] = ' '; }
      }

      if (!(lRelPos & 3)) lIndex--;

      szBuf[lIndex  ]   = '<';
      szBuf[lIndex+1]   = ' ';

      printf("%s\n", szBuf);

      buf.pData   += lOutLen;
      buf.lSize   -= lOutLen;
    }
}

std::string str_repeat(const std::string &str, size_t n) {
    std::string s;
    for(int i=0;i<n;++i) s += str;
    return s;
}

std::string str_xor(const std::string &a, const std::string &b) {
    std::string c;
    size_t len_a = a.length();
    size_t len_b = b.length();
    assert(len_a && len_b);

    if(len_a > len_b) {
        c = a;
        for(int i=0; i<len_a; ++i)
            c[i] ^= b[i % len_b];
    } else {
        c = b;
        for(int i=0; i<len_b; ++i)
            c[i] ^= a[i % len_a];
    }
    return c;
}
