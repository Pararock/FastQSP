/* Copyright (C) 2005-2010 Valeriy Argunov (nporep AT mail DOT ru) */
/*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "text.h"
#include "coding.h"
#include "errors.h"
#include "locations.h"
#include "mathops.h"
#include "variables.h"
#include "variant.h"
#include <QtDebug>
#include <vector>

// Что это за пиздец?! Кто так, блять, пишет? Теперь понятно, откуда тормоза. Срочно переписать.
int qspAddText(QSP_CHAR **dest, const QSP_CHAR *val, int destLen, int valLen,
               QSP_BOOL isCreate) {
  int ret;
  QSP_CHAR *destPtr;
  if (valLen < 0)
    valLen = qspStrLen(val);
  if (!isCreate && *dest) {
    if (destLen < 0)
      destLen = qspStrLen(*dest);
    ret = destLen + valLen;
    destPtr = (QSP_CHAR *)realloc(*dest, (ret + 1) * sizeof(QSP_CHAR));
    *dest = destPtr;
    destPtr += destLen;
  } else {
    ret = valLen;
    destPtr = (QSP_CHAR *)malloc((ret + 1) * sizeof(QSP_CHAR));
    *dest = destPtr;
  }
  qspStrNCopy(destPtr, val, valLen);
  destPtr[valLen] = 0;
  return ret;
}

// новая версия. Что это костыль - похуй. Главно быстро работает.
int qspAddText2(std::vector<QSP_CHAR> *dest, const QSP_CHAR *val, int valLen, QSP_BOOL isCreate)
{
    if (valLen < 0)
      valLen = qspStrLen(val);
    int destLen = dest->size()-1;
    // Удаляем символ завершения строки
    if((!dest->empty()) && ((*dest)[destLen] == 0))
        dest->pop_back();
    // Добавляем в конец
    for(int k = 0; k < valLen; k++){
        dest->push_back(val[k]);
    }
    // Добавляем символ завершения строки
    dest->push_back(0);
    return dest->size()-1;
}

QSP_CHAR* qspGetNewText(const QSP_CHAR *val, int valLen) {
  QSP_CHAR *buf;
  qspAddText(&buf, val, 0, valLen, QSP_TRUE);
  return buf;
}

QSP_CHAR *qspGetAddText(QSP_CHAR *dest, QSP_CHAR *val, int destLen,
                        int valLen) {
  qspAddText(&dest, val, destLen, valLen, QSP_FALSE);
  return dest;
}

QSP_BOOL qspClearText(QSP_CHAR **text, int *textLen) {
  if (*text) {
    free(*text);
    *text = nullptr;
    if (*textLen) {
      *textLen = 0;
      return QSP_TRUE;
    }
  }
  return QSP_FALSE;
}

QSP_BOOL qspIsInList(const QSP_CHAR *list, QSP_CHAR ch) {
  while (*list)
    if (*list++ == ch)
      return QSP_TRUE;
  return QSP_FALSE;
}

QSP_BOOL qspIsInListEOL(const QSP_CHAR *list, QSP_CHAR ch) {
  while (*list && *list != ch)
    ++list;
  return (*list == ch);
}

QSP_BOOL qspIsDigit(QSP_CHAR ch) {
  return (ch >= QSP_FMT('0') && ch <= QSP_FMT('9'));
}

const QSP_CHAR *qspSkipSpaces(const QSP_CHAR *s) {
  while (qspIsInList(QSP_SPACES, *s))
    ++s;
  return s;
}

QSP_CHAR *qspSkipSpaces(QSP_CHAR *s) {
  return const_cast<QSP_CHAR *>(qspSkipSpaces(const_cast<const QSP_CHAR *>(s)));
}

const QSP_CHAR *qspStrEnd(const QSP_CHAR *s) {
  while (*s)
    ++s;
  return s;
}

QSP_CHAR *qspDelSpc(QSP_CHAR *s) {
  int len;
  QSP_CHAR *str;
  const QSP_CHAR *begin = qspSkipSpaces(s), *end = qspStrEnd(begin);
  while (begin < end && qspIsInList(QSP_SPACES, *(end - 1)))
    --end;
  len = (int)(end - begin);
  str = (QSP_CHAR *)malloc((len + 1) * sizeof(QSP_CHAR));
  qspStrNCopy(str, begin, len);
  str[len] = 0;
  return str;
}

QSP_CHAR *qspDelSpcCanRetSelf(QSP_CHAR *s) {
  int len;
  QSP_CHAR *str;
  const QSP_CHAR *origEnd, *begin = qspSkipSpaces(s), *end = qspStrEnd(begin);
  origEnd = end;
  while (begin < end && qspIsInList(QSP_SPACES, *(end - 1)))
    --end;
  if (begin == s && end == origEnd)
    return s;
  len = (int)(end - begin);
  str = (QSP_CHAR *)malloc((len + 1) * sizeof(QSP_CHAR));
  qspStrNCopy(str, begin, len);
  str[len] = 0;
  return str;
}

QSP_BOOL qspIsAnyString(QSP_CHAR *s) { return (*qspSkipSpaces(s) != 0); }

void qspLowerStr(QSP_CHAR *str) {
  //	while (*str) *str++ = QSP_CHRLWR(*str);
  while (*str) {
    QSP_CHAR c = QSP_CHRLWR(*str);
    *str++ = c;
  }
}

void qspUpperStr(QSP_CHAR *str) {
  //    For some reason str == NULL after that code
  //    compiler mingw32-gcc-g++ 4.8.1-4
  //  while (*str) *str++ = QSP_CHRUPR(*str);
  while (*str) {
    QSP_CHAR c = QSP_CHRUPR(*str);
    *str++ = c;
  }
}

int qspStrsNComp(const QSP_CHAR *str1, const QSP_CHAR *str2, int maxLen) {
  int delta = 0;
  while (maxLen-- && !(delta = (int)*str1 - *str2) && *str2)
    ++str1, ++str2;
  return delta;
}

int qspStrsComp(const QSP_CHAR *str1, const QSP_CHAR *str2) {
  int ret = 0;
  while (!(ret = (int)*str1 - *str2) && *str2)
    ++str1, ++str2;
  if (ret < 0)
    return -1;
  else if (ret > 0)
    return 1;
  return 0;
}

QSP_CHAR *qspStrCopy(QSP_CHAR *strDest, const QSP_CHAR *strSource) {
  QSP_CHAR *ret = strDest;
  while (*strDest++ = *strSource++)
    ;
  return ret;
}

QSP_CHAR *qspStrChar(QSP_CHAR *str, QSP_CHAR ch) {
  while (*str && *str != ch)
    ++str;
  if (*str == ch)
    return str;
  return nullptr;
}

QSP_CHAR *qspStrNCopy(QSP_CHAR *strDest, const QSP_CHAR *strSource, int maxLen) {
  QSP_CHAR *ret = strDest;
  while (maxLen-- && (*strDest++ = *strSource++))
    ;
  return ret;
}

int qspStrLen(const QSP_CHAR *str) {
  const QSP_CHAR *bos = str;
  while (*str)
    ++str;
  return (int)(str - bos);
}

const QSP_CHAR *qspStrStr(const QSP_CHAR *str, const QSP_CHAR *strSearch) {
  const QSP_CHAR *s1, *s2;
  while (*str) {
    s1 = str;
    s2 = strSearch;
    while (*s1 && *s2 && !((int)*s1 - *s2))
      ++s1, ++s2;
    if (!(*s2))
      return str;
    ++str;
  }
  return nullptr;
}

QSP_CHAR *qspStrStr(QSP_CHAR *str, const QSP_CHAR *strSearch) {
  return const_cast<QSP_CHAR *>(qspStrStr(const_cast<const QSP_CHAR *>(str), strSearch));
}

const QSP_CHAR *qspStrPBrk(const QSP_CHAR *str, const QSP_CHAR *strCharSet) {
  const QSP_CHAR *set;
  while (*str) {
    for (set = strCharSet; *set; ++set)
      if (*set == *str)
        return str;
    ++str;
  }
  return nullptr;
}

QSP_CHAR *qspInStrRChars(QSP_CHAR *str, const QSP_CHAR *chars, QSP_CHAR *end) {
  QSP_CHAR *pos = nullptr;
  while (*str) {
    if (end && str == end)
      break;
    if (qspIsInList(chars, *str))
      pos = str;
    ++str;
  }
  return pos;
}

QSP_CHAR *qspJoinStrs(QSP_CHAR **s, int count, QSP_CHAR *delim) {
  int i, txtLen = 0, txtRealLen = 0, bufSize = 256, lastIndex = count - 1,
         delimLen = qspStrLen(delim);
  QSP_CHAR *txt = (QSP_CHAR *)malloc(bufSize * sizeof(QSP_CHAR));
  *txt = 0;
  for (i = 0; i < count; ++i) {
    if ((txtLen += qspStrLen(s[i])) >= bufSize) {
      bufSize = txtLen + 128;
      txt = (QSP_CHAR *)realloc(txt, bufSize * sizeof(QSP_CHAR));
    }
    qspStrCopy(txt + txtRealLen, s[i]);
    if (i == lastIndex)
      break;
    txtRealLen = txtLen;
    if ((txtLen += delimLen) >= bufSize) {
      bufSize = txtLen + 128;
      txt = (QSP_CHAR *)realloc(txt, bufSize * sizeof(QSP_CHAR));
    }
    qspStrCopy(txt + txtRealLen, delim);
    txtRealLen = txtLen;
  }
  return txt;
}
// последний параметр - предполагаемое число строк
int qspSplitStr(QSP_CHAR *str, const QSP_CHAR *delim, QSP_CHAR ***res, int strEval) {
  int allocChars, count = 0, bufSize = strEval, delimLen = qspStrLen(delim);
  QSP_CHAR *newStr, **ret, *curPos = str, *found = qspStrStr(str, delim);
  ret = (QSP_CHAR **)malloc(bufSize * sizeof(QSP_CHAR *));
  while (found) {
    allocChars = (int)(found - curPos);
    newStr = (QSP_CHAR *)malloc((allocChars + 1) * sizeof(QSP_CHAR));
    memcpy(newStr, curPos, allocChars * sizeof (QSP_CHAR));
    newStr[allocChars] = 0;
    if (++count > bufSize) {
      bufSize += 1024;
      ret = (QSP_CHAR **)realloc(ret, bufSize * sizeof(QSP_CHAR *));
    }
    ret[count - 1] = newStr;
    curPos = found + delimLen;
    found = qspStrStr(curPos, delim);
  }
  newStr = (QSP_CHAR *)malloc((qspStrLen(curPos) + 1) * sizeof(QSP_CHAR));
  qspStrCopy(newStr, curPos);
  if (++count > bufSize)
    ret = (QSP_CHAR **)realloc(ret, count * sizeof(QSP_CHAR *));
  ret[count - 1] = newStr;
  *res = ret;
  return count;
}

void qspCopyStrs(QSP_CHAR ***dest, QSP_CHAR **src, int start, int end) {
  int i, count = end - start;
  if (src && count) {
    *dest = (QSP_CHAR **)malloc(count * sizeof(QSP_CHAR *));
    i = 0;
    while (start < end)
      qspAddText(*dest + i++, src[start++], 0, -1, QSP_TRUE);
  } else
    *dest = nullptr;
}

QSP_BOOL qspIsNumber(QSP_CHAR *s) {
  s = qspSkipSpaces(s);
  if (*s == QSP_FMT('-'))
    ++s;
  else if (*s == QSP_FMT('+'))
    ++s;
  if (qspIsDigit(*s)) {
    do {
      ++s;
    } while (qspIsDigit(*s));
  } else
    return QSP_FALSE;
  s = qspSkipSpaces(s);
  return !(*s);
}

int qspStrToNum(QSP_CHAR *s, QSP_BOOL *isValid) {
  int num;
  QSP_BOOL isNeg = QSP_FALSE;
  s = qspSkipSpaces(s);
  if (*s == QSP_FMT('-')) {
    isNeg = QSP_TRUE;
    ++s;
  } else if (*s == QSP_FMT('+'))
    ++s;
  else if (!(*s)) /* special case, i.e. empty string must be convertible to 0 */
  {
    if (isValid)
      *isValid = QSP_TRUE;
    return 0;
  }
  if (qspIsDigit(*s)) {
    num = 0;
    do {
      num = num * 10 + (*s - QSP_FMT('0'));
      ++s;
    } while (qspIsDigit(*s));
  } else {
    if (isValid)
      *isValid = QSP_FALSE;
    return 0;
  }
  s = qspSkipSpaces(s);
  if (*s) {
    if (isValid)
      *isValid = QSP_FALSE;
    return 0;
  }
  if (isValid)
    *isValid = QSP_TRUE;
  if (isNeg)
    return -num;
  return num;
}

QSP_CHAR *qspNumToStr(QSP_CHAR *buf, int val) {
  QSP_CHAR temp, *str = buf, *first = str;
  if (val < 0) {
    *str++ = QSP_FMT('-');
    val = -val;
    ++first;
  }
  do {
    *str++ = (QSP_CHAR)(val % 10 + QSP_FMT('0'));
    val /= 10;
  } while (val > 0);
  *str-- = 0;
  while (first < str) {
    temp = *str;
    *str = *first;
    *first = temp;
    --str;
    ++first;
  }
  return buf;
}

const QSP_CHAR *qspStrPos(const QSP_CHAR *txt, const QSP_CHAR *str, QSP_BOOL isIsolated) {
  QSP_BOOL isLastDelim;
  int strLen, c1, c2, c3;
  QSP_CHAR quot;
  const QSP_CHAR *pos = qspStrStr(txt, str);
  if (!pos)
    return nullptr;
  if (!(isIsolated ||
        qspStrPBrk(txt, QSP_QUOTS QSP_LQUOT QSP_LRBRACK QSP_LSBRACK)))
    return pos;
  strLen = qspStrLen(str);
  pos = qspStrEnd(txt) - strLen + 1;
  c1 = c2 = c3 = 0;
  isLastDelim = QSP_TRUE;
  while (txt < pos) {
    if (qspIsInList(QSP_QUOTS, *txt)) {
      quot = *txt;
      while (++txt < pos)
        if (*txt == quot && *(++txt) != quot)
          break;
      if (txt >= pos)
        return nullptr;
      isLastDelim = QSP_TRUE;
    }
    if (*txt == QSP_LRBRACK[0])
      ++c1;
    else if (*txt == QSP_RRBRACK[0]) {
      if (c1)
        --c1;
    } else if (*txt == QSP_LSBRACK[0])
      ++c2;
    else if (*txt == QSP_RSBRACK[0]) {
      if (c2)
        --c2;
    } else if (*txt == QSP_LQUOT[0])
      ++c3;
    else if (*txt == QSP_RQUOT[0]) {
      if (c3)
        --c3;
    }
    if (!(c1 || c2 || c3)) {
      if (isIsolated) {
        if (qspIsInList(QSP_DELIMS, *txt))
          isLastDelim = QSP_TRUE;
        else if (isLastDelim) {
          if (qspIsInListEOL(QSP_DELIMS, txt[strLen]) &&
              !qspStrsNComp(txt, str, strLen))
            return txt;
          isLastDelim = QSP_FALSE;
        }
      } else if (!qspStrsNComp(txt, str, strLen))
        return txt;
    }
    ++txt;
  }
  return nullptr;
}

QSP_CHAR *qspStrPos(QSP_CHAR *txt, const QSP_CHAR *str, QSP_BOOL isIsolated) {
  return const_cast<QSP_CHAR *>(qspStrPos(const_cast<const QSP_CHAR *>(txt), str, isIsolated));
}

QSP_CHAR *qspStrPosPartial(QSP_CHAR *txt, QSP_CHAR *pos, const QSP_CHAR *str,
                           QSP_BOOL isIsolated) {
  QSP_CHAR ch, *res;
  if (pos) {
    ch = *pos;
    *pos = 0;
    res = qspStrPos(txt, str, isIsolated);
    *pos = ch;
    return res;
  }
  return qspStrPos(txt, str, isIsolated);
}

QSP_CHAR *qspReplaceText(QSP_CHAR *txt, const QSP_CHAR *searchTxt, const QSP_CHAR *repTxt) {
  int txtLen, oldTxtLen, bufSize, searchLen, repLen, len;
  QSP_CHAR *newTxt, *pos = qspStrStr(txt, searchTxt);
  if (!pos)
    return qspGetNewText(txt, -1);
  bufSize = 256;
  txtLen = oldTxtLen = 0;
  searchLen = qspStrLen(searchTxt);
  repLen = qspStrLen(repTxt);
  newTxt = (QSP_CHAR *)malloc(bufSize * sizeof(QSP_CHAR));
  do {
    len = (int)(pos - txt);
    if ((txtLen += len + repLen) >= bufSize) {
      bufSize = txtLen + 128;
      newTxt = (QSP_CHAR *)realloc(newTxt, bufSize * sizeof(QSP_CHAR));
    }
    qspStrNCopy(newTxt + oldTxtLen, txt, len);
    qspStrCopy(newTxt + oldTxtLen + len, repTxt);
    oldTxtLen = txtLen;
    txt = pos + searchLen;
    pos = qspStrStr(txt, searchTxt);
  } while (pos);
  return qspGetAddText(newTxt, txt, txtLen, -1);
}

QSP_CHAR *qspFormatText(QSP_CHAR *txt, QSP_BOOL canReturnSelf) {
  QSPVariant val;
  QSP_CHAR *newTxt, *lPos, *rPos;
  int oldRefreshCount, len, txtLen, oldTxtLen, bufSize;
  if (qspGetVarNumValue(QSP_FMT("DISABLESUBEX"))) {
    if (canReturnSelf)
      return txt;
    return qspGetNewText(txt, -1);
  }
  lPos = qspStrStr(txt, QSP_LSUBEX);
  if (!lPos) {
    if (canReturnSelf)
      return txt;
    return qspGetNewText(txt, -1);
  }
  bufSize = 256;
  newTxt = (QSP_CHAR *)malloc(bufSize * sizeof(QSP_CHAR));
  txtLen = oldTxtLen = 0;
  oldRefreshCount = qspRefreshCount;
  do {
    len = (int)(lPos - txt);
    if ((txtLen += len) >= bufSize) {
      bufSize = txtLen + 128;
      newTxt = (QSP_CHAR *)realloc(newTxt, bufSize * sizeof(QSP_CHAR));
    }
    qspStrNCopy(newTxt + oldTxtLen, txt, len);
    oldTxtLen = txtLen;
    txt = lPos + QSP_LEN(QSP_LSUBEX);
    rPos = qspStrPos(txt, QSP_RSUBEX, QSP_FALSE);
    if (!rPos) {
      qspSetError(QSP_ERR_BRACKNOTFOUND);
      free(newTxt);
      return nullptr;
    }
    *rPos = 0;
    val = qspExprValue(txt);
    *rPos = QSP_RSUBEX[0];
    if (qspRefreshCount != oldRefreshCount || qspErrorNum) {
      free(newTxt);
      return nullptr;
    }
    qspConvertVariantTo(&val, QSP_TRUE);
    if ((txtLen += qspStrLen(QSP_STR(val))) >= bufSize) {
      bufSize = txtLen + 128;
      newTxt = (QSP_CHAR *)realloc(newTxt, bufSize * sizeof(QSP_CHAR));
    }
    qspStrCopy(newTxt + oldTxtLen, QSP_STR(val));
    free(QSP_STR(val));
    oldTxtLen = txtLen;
    txt = rPos + QSP_LEN(QSP_RSUBEX);
    lPos = qspStrStr(txt, QSP_LSUBEX);
  } while (lPos);
  return qspGetAddText(newTxt, txt, txtLen, -1);
}
