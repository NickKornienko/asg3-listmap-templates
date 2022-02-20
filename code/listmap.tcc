// $Id: listmap.tcc,v 1.15 2019-10-30 12:44:53-07 - - $

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t, mapped_t, less_t>::~listmap()
{
   DEBUGF('l', reinterpret_cast<const void *>(this));
}

//
// iterator listmap::insert (const value_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t, mapped_t, less_t>::iterator
listmap<key_t, mapped_t, less_t>::insert(const value_type &pair)
{
   for (iterator itr = begin(); itr != end(); ++itr)
   {
      if (less(pair.first, itr->first))
      {
         if (less(itr->first, pair.first))
         {
            itr->second = pair.second;
         }
         node *curr = ++itr.where; // might not work
         node to_insert(curr->next, curr->prev, pair);
         return itr;
      }
   }
   iterator itr = end();
   node *curr = --itr.where; // might not work
   node to_insert(curr->next, curr->prev, pair);
   return itr;
}

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t, mapped_t, less_t>::iterator
listmap<key_t, mapped_t, less_t>::find(const key_type &that)
{
   for (iterator itr = begin(); itr != end(); ++itr)
   {
      if (less(that, itr->first) && less(itr->first, that))
      {
         return itr;
      }
   }
   return end();
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t, mapped_t, less_t>::iterator
listmap<key_t, mapped_t, less_t>::erase(iterator position)
{
   DEBUGF('l', &*position);
   return iterator();
}

template <typename key_t, typename mapped_t, class less_t>
void listmap<key_t, mapped_t, less_t>::print(const key_type &key, const iterator &that)
{
   iterator itr = that;
   if (itr == end())
   {
      cout << key << ": key not found" << endl;
      return;
   }
   cout << itr->first << " " << itr->second << endl;
}

template <typename key_t, typename mapped_t, class less_t>
void listmap<key_t, mapped_t, less_t>::print_all()
{
   for (iterator itr = begin(); itr != end(); ++itr)
   {
      print(itr->first, itr);
   }
}