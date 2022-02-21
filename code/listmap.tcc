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
   while (!empty())
   {
      erase(begin());
   }
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
      if (!less(itr->first, pair.first))
      {
         if (!less(pair.first, itr->first))
         {
            itr->second = pair.second;
            return itr;
         }
         node *curr = itr.where;
         node *to_insert = new node(curr, curr->prev, pair);

         curr->prev->next = to_insert;
         curr->prev = to_insert;
         return --itr;
      }
   }
   iterator itr = end();
   node *anc = itr.where;

   node *to_insert = new node(anc, anc->prev, pair);

   anc->prev->next = to_insert;
   anc->prev = to_insert;
   --itr;

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
      if (!less(that, itr->first) && !less(itr->first, that))
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
   if (position == end())
   {
      return end();
   }

   node *to_delete = position.where;
   to_delete->prev->next = to_delete->next;
   to_delete->next->prev = to_delete->prev;

   delete to_delete;

   return ++position;
}

template <typename key_t, typename mapped_t, class less_t>
void listmap<key_t, mapped_t, less_t>::print(const key_type &key,
                                             const iterator &that)
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
