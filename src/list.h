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

/* This file is part of KDevelop
    Copyright (C) 2002-2005 Roberto Raggi <roberto@kdevelop.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef FASTLIST_H
#define FASTLIST_H

#include "smallobject.h"

template <typename Tp>
struct ListNode
{
  Tp element;
  int index;
  mutable const ListNode<Tp> *next;

  static ListNode *create(const Tp &element, pool *p)
  {
    ListNode<Tp> *node = new (p->allocate(sizeof(ListNode))) ListNode();
    node->element = element;
    node->index = 0;
    node->next = node;

    return node;
  }

  static ListNode *create(const ListNode *n1, const Tp &element, pool *p)
  {
    ListNode<Tp> *n2 = ListNode::create(element, p);

    n2->index = n1->index + 1;
    n2->next = n1->next;
    n1->next = n2;

    return n2;
  }

  inline ListNode<Tp>() { }

  inline const ListNode<Tp> *at(int index) const
  {
    const ListNode<Tp> *node = this;
    while (index != node->index)
      node = node->next;

    return node;
  }

  inline bool hasNext() const
  { return index < next->index; }

  inline int count() const
  { return 1 + toBack()->index; }

  inline const ListNode<Tp> *toFront() const
  { return toBack()->next; }

  inline const ListNode<Tp> *toBack() const
  {
    const ListNode<Tp> *node = this;
    while (node->hasNext())
      node = node->next;

    return node;
  }
};

template <class Tp>
inline const ListNode<Tp> *snoc(const ListNode<Tp> *list,
				const Tp &element, pool *p)
{
  if (!list)
    return ListNode<Tp>::create(element, p);

  return ListNode<Tp>::create(list->toBack(), element, p);
}

#endif // FASTLIST_H

// kate: space-indent on; indent-width 2; replace-tabs on;

