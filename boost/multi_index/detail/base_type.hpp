/* Copyright 2003-2013 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#pragma once

#include <brigand/functions/eval_if.hpp>
#include <brigand/functions/arithmetic/identity.hpp>
//#include <brigand/sequences/at.hpp>
//#include <brigand/sequences/size.hpp>
// TODO: needs to be brigand apply
#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/multi_index/detail/index_base.hpp>
#include <boost/multi_index/detail/is_index_list.hpp>

namespace boost{

namespace multi_index{

namespace detail{

/* MPL machinery to construct a linear hierarchy of indices out of
 * a index list.
 */

struct index_applier
{
  template<typename IndexSpecifierMeta,typename SuperMeta>
  struct apply
  {
    typedef typename IndexSpecifierMeta::type            index_specifier;
    typedef typename index_specifier::
      template index_class<SuperMeta>::type type;
  };
};

template<int N,typename Value,typename IndexSpecifierList,typename Allocator>
struct nth_layer
{
  static const int length = mpl::size<IndexSpecifierList>::value;

  typedef typename  brigand::eval_if_c<
    N==length,
    brigand::identity<index_base<Value,IndexSpecifierList,Allocator> >,
    mpl::apply2<
      index_applier,
      mpl::at_c<IndexSpecifierList,N>,
      nth_layer<N+1,Value,IndexSpecifierList,Allocator>
    >
  >::type type;
};

template<typename Value,typename IndexSpecifierList,typename Allocator>
struct multi_index_base_type:nth_layer<0,Value,IndexSpecifierList,Allocator>
{
  static_assert(detail::is_index_list<IndexSpecifierList>::value, "");
};

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace boost */
