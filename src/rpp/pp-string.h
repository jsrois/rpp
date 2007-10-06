/****************************************************************************
**
** Copyright (C) 1992-2007 Trolltech ASA. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

/*
  Copyright 2005 Roberto Raggi <roberto@kdevelop.org>

  Permission to use, copy, modify, distribute, and sell this software and its
  documentation for any purpose is hereby granted without fee, provided that
  the above copyright notice appear in all copies and that both that
  copyright notice and this permission notice appear in supporting
  documentation.

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  KDEVELOP TEAM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
  AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef PP_STRING_H
#define PP_STRING_H

namespace rpp {

template <typename _CharT>
class pp_string
{
  typedef std::char_traits<_CharT> traits_type;
  typedef std::size_t size_type;

  _CharT const *_M_begin;
  std::size_t _M_size;

public:
  inline pp_string ():
    _M_begin (0), _M_size(0) {}

  explicit pp_string (std::string const &__s):
    _M_begin (__s.c_str ()), _M_size (__s.size ()) {}

  inline pp_string (_CharT const *__begin, std::size_t __size):
    _M_begin (__begin), _M_size (__size) {}

  inline _CharT const *begin () const { return _M_begin; }
  inline _CharT const *end () const { return _M_begin + _M_size; }

  inline _CharT at (std::size_t index) const { return _M_begin [index]; }

  inline std::size_t size () const { return _M_size; }

  inline int compare (pp_string const &__other) const
  {
    size_type const __size = this->size();
    size_type const __osize = __other.size();
    size_type const __len = std::min (__size,  __osize);

    int __r = traits_type::compare (_M_begin, __other._M_begin, __len);
    if (!__r)
        __r =  (int) (__size - __osize);

    return __r;
  }

  inline bool operator == (pp_string const &__other) const
  { return compare (__other) == 0; }

  inline bool operator != (pp_string const &__other) const
  { return compare (__other) != 0; }

  inline bool operator < (pp_string const &__other) const
  { return compare (__other) < 0; }

  inline bool operator == (char const *s) const
  {
    std::size_t n = strlen (s);

    if (n != _M_size)
      return false;

    return ! strncmp (_M_begin, s, n);
  }

  inline bool operator != (char const *s) const
  { return ! operator == (s); }
};

} // namespace rpp

#endif // PP_STRING_H

// kate: space-indent on; indent-width 2; replace-tabs on;
