#include <CGAL/basic.h>
#include <iostream>

#include <CGAL/Arr_tags.h>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

struct Traits1 {
  typedef CGAL::Arr_open_side_tag Arr_left_side_tag;
  typedef CGAL::Arr_open_side_tag Arr_bottom_side_tag;
  typedef CGAL::Arr_open_side_tag Arr_top_side_tag;
  typedef CGAL::Arr_open_side_tag Arr_right_side_tag;
};

struct Traits2 {
  typedef CGAL::Arr_open_side_tag Arr_left_side_tag;
  typedef CGAL::Arr_closed_side_tag Arr_bottom_side_tag;
  typedef CGAL::Arr_open_side_tag Arr_top_side_tag;
  typedef CGAL::Arr_open_side_tag Arr_right_side_tag;
};

struct Traits3 {
  typedef CGAL::Arr_open_side_tag Arr_left_side_tag;
  typedef CGAL::Arr_open_side_tag Arr_bottom_side_tag;
  typedef CGAL::Arr_closed_side_tag Arr_top_side_tag;
  typedef CGAL::Arr_open_side_tag Arr_right_side_tag;
};

struct Traits4 {
  typedef CGAL::Arr_open_side_tag Arr_left_side_tag;
  typedef CGAL::Arr_open_side_tag Arr_bottom_side_tag;
  typedef CGAL::Arr_open_side_tag Arr_top_side_tag;
  typedef CGAL::Arr_closed_side_tag Arr_right_side_tag;
};


bool dispatch(CGAL::Arr_all_sides_oblivious_tag) {
  return true;
}

bool dispatch(CGAL::Arr_not_all_sides_oblivious_tag) {
  return false;
}

bool dispatch(CGAL::Arr_all_sides_non_open_tag) {
  return true;
}

bool dispatch(CGAL::Arr_not_all_sides_non_open_tag) {
  return false;
}

int main ()
{

  typedef CGAL::Arr_are_all_sides_oblivious_tag< 
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag >::result result1;

  assert(dispatch(result1()));

  typedef CGAL::Arr_are_all_sides_oblivious_tag< 
    CGAL::Arr_open_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag >::result result2;

  assert(!dispatch(result2()));

  typedef CGAL::Arr_are_all_sides_oblivious_tag< 
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag >::result result3;

  assert(!dispatch(result3()));

  typedef CGAL::Arr_are_all_sides_oblivious_tag< 
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_oblivious_side_tag >::result result4;

  assert(!dispatch(result4()));

  typedef CGAL::Arr_are_all_sides_oblivious_tag< 
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_open_side_tag >::result result5;
  
  assert(!dispatch(result5()));

  typedef CGAL::Arr_are_all_sides_oblivious_tag< 
    CGAL::Arr_closed_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_open_side_tag >::result result6;

  assert(!dispatch(result6()));

  typedef CGAL::Arr_are_all_sides_oblivious_tag< 
    CGAL::Arr_contracted_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_closed_side_tag >::result result7;

  assert(!dispatch(result7()));

  // all non-open

  typedef CGAL::Arr_are_all_sides_non_open_tag< 
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag >::result open1;

  assert(dispatch(open1()));

  typedef CGAL::Arr_are_all_sides_non_open_tag< 
    CGAL::Arr_open_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag >::result open2;

  assert(!dispatch(open2()));

  typedef CGAL::Arr_are_all_sides_non_open_tag< 
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag >::result open3;

  assert(!dispatch(open3()));
  
  typedef CGAL::Arr_are_all_sides_non_open_tag< 
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_oblivious_side_tag >::result open4;

  assert(!dispatch(open4()));

  typedef CGAL::Arr_are_all_sides_non_open_tag< 
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_open_side_tag >::result open5;

  assert(!dispatch(open5()));

  typedef CGAL::Arr_are_all_sides_non_open_tag< 
    CGAL::Arr_closed_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_open_side_tag >::result open6;

  assert(!dispatch(open6()));
  
  typedef CGAL::Arr_are_all_sides_non_open_tag< 
    CGAL::Arr_open_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_open_side_tag >::result open7;

  assert(!dispatch(open7()));
  
  // opposite identified tagging

  typedef CGAL::Arr_sane_identified_tagging< 
    CGAL::Arr_open_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_closed_side_tag >::result ident1;

  assert(ident1() == true);

  typedef CGAL::Arr_sane_identified_tagging< 
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_oblivious_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_identified_side_tag >::result ident2;

  assert(ident2() == true);

  typedef CGAL::Arr_sane_identified_tagging< 
    CGAL::Arr_open_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_closed_side_tag >::result ident3;

  assert(ident3() == true);

  typedef CGAL::Arr_sane_identified_tagging< 
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_identified_side_tag >::result ident4;

  assert(ident4() == true);


  typedef CGAL::Arr_sane_identified_tagging< 
    CGAL::Arr_closed_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_identified_side_tag >::result ident5;

  assert(ident5() == false);

  typedef CGAL::Arr_sane_identified_tagging< 
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_identified_side_tag >::result ident6;

  assert(ident6() == false);

  typedef CGAL::Arr_sane_identified_tagging< 
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_contracted_side_tag,
    CGAL::Arr_identified_side_tag >::result ident7;

  assert(ident7() == false);

  typedef CGAL::Arr_sane_identified_tagging< 
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_oblivious_side_tag >::result ident8;

  assert(ident8() == false);

  typedef CGAL::Arr_sane_identified_tagging< 
    CGAL::Arr_closed_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_identified_side_tag >::result ident9;

  assert(ident9() == false);

  typedef CGAL::Arr_sane_identified_tagging< 
    CGAL::Arr_open_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_open_side_tag >::result ident10;

  assert(ident10() == false);

  typedef CGAL::Arr_sane_identified_tagging< 
    CGAL::Arr_open_side_tag,
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_contracted_side_tag,
    CGAL::Arr_open_side_tag >::result ident11;

  assert(ident11() == false);

  typedef CGAL::Arr_sane_identified_tagging< 
    CGAL::Arr_identified_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_open_side_tag,
    CGAL::Arr_oblivious_side_tag >::result ident12;

  assert(ident12() == false);


  typedef CGAL::Arr_same_tags_in_traits_classes< Traits1, Traits1 >::result
    equal1;

  assert(equal1() == true);

  typedef CGAL::Arr_same_tags_in_traits_classes< Traits1, Traits2 >::result
    equal2;

  assert(equal2() == false);

  typedef CGAL::Arr_same_tags_in_traits_classes< Traits1, Traits3 >::result
    equal3;

  assert(equal3() == false);

  typedef CGAL::Arr_same_tags_in_traits_classes< Traits1, Traits4 >::result
    equal4;

  assert(equal4() == false);

  return EXIT_SUCCESS;

}