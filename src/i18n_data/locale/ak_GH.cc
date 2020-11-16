#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Akan locale for Ghana"),.source=tsc("Sugar Labs // OLPC\t\t;\t\tfast_io"),.address=tsc("https://github.com/expnkx/fast_io"),.contact=tsc("sugarlabs.org\t\t;\t\tfast_io"),.email=tsc("libc-alpha@sourceware.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Akan"),.territory=tsc("Ghana"),.revision=tsc("1.0"),.date=tsc("2013-08-24")},.monetary={.int_curr_symbol=tsc("GHS "),.currency_symbol=tsc("GH₵"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("Kwe"),tsc("Dwo"),tsc("Ben"),tsc("Wuk"),tsc("Yaw"),tsc("Fia"),tsc("Mem")},.day={tsc("Kwesida"),tsc("Dwowda"),tsc("Benada"),tsc("Wukuda"),tsc("Yawda"),tsc("Fida"),tsc("Memeneda")},.abmon={tsc("S-Ɔ"),tsc("K-Ɔ"),tsc("E-Ɔ"),tsc("E-O"),tsc("E-K"),tsc("O-A"),tsc("A-K"),tsc("D-Ɔ"),tsc("F-Ɛ"),tsc("Ɔ-A"),tsc("Ɔ-O"),tsc("M-Ɔ")},.mon={tsc("Sanda-Ɔpɛpɔn"),tsc("Kwakwar-Ɔgyefuo"),tsc("Ebɔw-Ɔbenem"),tsc("Ebɔbira-Oforisuo"),tsc("Esusow Aketseaba-Kɔtɔnimba"),tsc("Obirade-Ayɛwohomumu"),tsc("Ayɛwoho-Kitawonsa"),tsc("Difuu-Ɔsandaa"),tsc("Fankwa-Ɛbɔ"),tsc("Ɔbɛsɛ-Ahinime"),tsc("Ɔberɛfɛw-Obubuo"),tsc("Mumu-Ɔpɛnimba")},.d_t_fmt=tsc("%a %d %b %Y %T"),.d_fmt=tsc("%Y//%m//%d"),.t_fmt=tsc("%r"),.t_fmt_ampm=tsc("%I:%M:%S %p"),.date_fmt=tsc("%a %d %b %Y %T %Z"),.am_pm={tsc("AN"),tsc("EW")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc("^[+1yY]"),.noexpr=tsc("^[-0dDnN]"),.yesstr=tsc("Yiw"),.nostr=tsc("Daabi")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.tel_dom_fmt=tsc("%a %l"),.int_select=tsc("00"),.int_prefix=tsc("233")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc("Gaana"),.country_post=tsc("GH"),.country_ab2=tsc("GH"),.country_ab3=tsc("GHA"),.country_num=288,.country_car=tsc("GH"),.country_isbn=tsc("9964"),.lang_name=tsc("Akan"),.lang_ab=tsc("ak"),.lang_term=tsc("aka")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Akan locale for Ghana"),.source=tsc(L"Sugar Labs // OLPC\t\t;\t\tfast_io"),.address=tsc(L"https://github.com/expnkx/fast_io"),.contact=tsc(L"sugarlabs.org\t\t;\t\tfast_io"),.email=tsc(L"libc-alpha@sourceware.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Akan"),.territory=tsc(L"Ghana"),.revision=tsc(L"1.0"),.date=tsc(L"2013-08-24")},.monetary={.int_curr_symbol=tsc(L"GHS "),.currency_symbol=tsc(L"GH₵"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"Kwe"),tsc(L"Dwo"),tsc(L"Ben"),tsc(L"Wuk"),tsc(L"Yaw"),tsc(L"Fia"),tsc(L"Mem")},.day={tsc(L"Kwesida"),tsc(L"Dwowda"),tsc(L"Benada"),tsc(L"Wukuda"),tsc(L"Yawda"),tsc(L"Fida"),tsc(L"Memeneda")},.abmon={tsc(L"S-Ɔ"),tsc(L"K-Ɔ"),tsc(L"E-Ɔ"),tsc(L"E-O"),tsc(L"E-K"),tsc(L"O-A"),tsc(L"A-K"),tsc(L"D-Ɔ"),tsc(L"F-Ɛ"),tsc(L"Ɔ-A"),tsc(L"Ɔ-O"),tsc(L"M-Ɔ")},.mon={tsc(L"Sanda-Ɔpɛpɔn"),tsc(L"Kwakwar-Ɔgyefuo"),tsc(L"Ebɔw-Ɔbenem"),tsc(L"Ebɔbira-Oforisuo"),tsc(L"Esusow Aketseaba-Kɔtɔnimba"),tsc(L"Obirade-Ayɛwohomumu"),tsc(L"Ayɛwoho-Kitawonsa"),tsc(L"Difuu-Ɔsandaa"),tsc(L"Fankwa-Ɛbɔ"),tsc(L"Ɔbɛsɛ-Ahinime"),tsc(L"Ɔberɛfɛw-Obubuo"),tsc(L"Mumu-Ɔpɛnimba")},.d_t_fmt=tsc(L"%a %d %b %Y %T"),.d_fmt=tsc(L"%Y//%m//%d"),.t_fmt=tsc(L"%r"),.t_fmt_ampm=tsc(L"%I:%M:%S %p"),.date_fmt=tsc(L"%a %d %b %Y %T %Z"),.am_pm={tsc(L"AN"),tsc(L"EW")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1yY]"),.noexpr=tsc(L"^[-0dDnN]"),.yesstr=tsc(L"Yiw"),.nostr=tsc(L"Daabi")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.tel_dom_fmt=tsc(L"%a %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"233")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc(L"Gaana"),.country_post=tsc(L"GH"),.country_ab2=tsc(L"GH"),.country_ab3=tsc(L"GHA"),.country_num=288,.country_car=tsc(L"GH"),.country_isbn=tsc(L"9964"),.lang_name=tsc(L"Akan"),.lang_ab=tsc(L"ak"),.lang_term=tsc(L"aka")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Akan locale for Ghana"),.source=tsc(u8"Sugar Labs // OLPC\t\t;\t\tfast_io"),.address=tsc(u8"https://github.com/expnkx/fast_io"),.contact=tsc(u8"sugarlabs.org\t\t;\t\tfast_io"),.email=tsc(u8"libc-alpha@sourceware.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Akan"),.territory=tsc(u8"Ghana"),.revision=tsc(u8"1.0"),.date=tsc(u8"2013-08-24")},.monetary={.int_curr_symbol=tsc(u8"GHS "),.currency_symbol=tsc(u8"GH₵"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"Kwe"),tsc(u8"Dwo"),tsc(u8"Ben"),tsc(u8"Wuk"),tsc(u8"Yaw"),tsc(u8"Fia"),tsc(u8"Mem")},.day={tsc(u8"Kwesida"),tsc(u8"Dwowda"),tsc(u8"Benada"),tsc(u8"Wukuda"),tsc(u8"Yawda"),tsc(u8"Fida"),tsc(u8"Memeneda")},.abmon={tsc(u8"S-Ɔ"),tsc(u8"K-Ɔ"),tsc(u8"E-Ɔ"),tsc(u8"E-O"),tsc(u8"E-K"),tsc(u8"O-A"),tsc(u8"A-K"),tsc(u8"D-Ɔ"),tsc(u8"F-Ɛ"),tsc(u8"Ɔ-A"),tsc(u8"Ɔ-O"),tsc(u8"M-Ɔ")},.mon={tsc(u8"Sanda-Ɔpɛpɔn"),tsc(u8"Kwakwar-Ɔgyefuo"),tsc(u8"Ebɔw-Ɔbenem"),tsc(u8"Ebɔbira-Oforisuo"),tsc(u8"Esusow Aketseaba-Kɔtɔnimba"),tsc(u8"Obirade-Ayɛwohomumu"),tsc(u8"Ayɛwoho-Kitawonsa"),tsc(u8"Difuu-Ɔsandaa"),tsc(u8"Fankwa-Ɛbɔ"),tsc(u8"Ɔbɛsɛ-Ahinime"),tsc(u8"Ɔberɛfɛw-Obubuo"),tsc(u8"Mumu-Ɔpɛnimba")},.d_t_fmt=tsc(u8"%a %d %b %Y %T"),.d_fmt=tsc(u8"%Y//%m//%d"),.t_fmt=tsc(u8"%r"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p"),.date_fmt=tsc(u8"%a %d %b %Y %T %Z"),.am_pm={tsc(u8"AN"),tsc(u8"EW")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1yY]"),.noexpr=tsc(u8"^[-0dDnN]"),.yesstr=tsc(u8"Yiw"),.nostr=tsc(u8"Daabi")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.tel_dom_fmt=tsc(u8"%a %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"233")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc(u8"Gaana"),.country_post=tsc(u8"GH"),.country_ab2=tsc(u8"GH"),.country_ab3=tsc(u8"GHA"),.country_num=288,.country_car=tsc(u8"GH"),.country_isbn=tsc(u8"9964"),.lang_name=tsc(u8"Akan"),.lang_ab=tsc(u8"ak"),.lang_term=tsc(u8"aka")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Akan locale for Ghana"),.source=tsc(u"Sugar Labs // OLPC\t\t;\t\tfast_io"),.address=tsc(u"https://github.com/expnkx/fast_io"),.contact=tsc(u"sugarlabs.org\t\t;\t\tfast_io"),.email=tsc(u"libc-alpha@sourceware.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Akan"),.territory=tsc(u"Ghana"),.revision=tsc(u"1.0"),.date=tsc(u"2013-08-24")},.monetary={.int_curr_symbol=tsc(u"GHS "),.currency_symbol=tsc(u"GH₵"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"Kwe"),tsc(u"Dwo"),tsc(u"Ben"),tsc(u"Wuk"),tsc(u"Yaw"),tsc(u"Fia"),tsc(u"Mem")},.day={tsc(u"Kwesida"),tsc(u"Dwowda"),tsc(u"Benada"),tsc(u"Wukuda"),tsc(u"Yawda"),tsc(u"Fida"),tsc(u"Memeneda")},.abmon={tsc(u"S-Ɔ"),tsc(u"K-Ɔ"),tsc(u"E-Ɔ"),tsc(u"E-O"),tsc(u"E-K"),tsc(u"O-A"),tsc(u"A-K"),tsc(u"D-Ɔ"),tsc(u"F-Ɛ"),tsc(u"Ɔ-A"),tsc(u"Ɔ-O"),tsc(u"M-Ɔ")},.mon={tsc(u"Sanda-Ɔpɛpɔn"),tsc(u"Kwakwar-Ɔgyefuo"),tsc(u"Ebɔw-Ɔbenem"),tsc(u"Ebɔbira-Oforisuo"),tsc(u"Esusow Aketseaba-Kɔtɔnimba"),tsc(u"Obirade-Ayɛwohomumu"),tsc(u"Ayɛwoho-Kitawonsa"),tsc(u"Difuu-Ɔsandaa"),tsc(u"Fankwa-Ɛbɔ"),tsc(u"Ɔbɛsɛ-Ahinime"),tsc(u"Ɔberɛfɛw-Obubuo"),tsc(u"Mumu-Ɔpɛnimba")},.d_t_fmt=tsc(u"%a %d %b %Y %T"),.d_fmt=tsc(u"%Y//%m//%d"),.t_fmt=tsc(u"%r"),.t_fmt_ampm=tsc(u"%I:%M:%S %p"),.date_fmt=tsc(u"%a %d %b %Y %T %Z"),.am_pm={tsc(u"AN"),tsc(u"EW")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1yY]"),.noexpr=tsc(u"^[-0dDnN]"),.yesstr=tsc(u"Yiw"),.nostr=tsc(u"Daabi")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.tel_dom_fmt=tsc(u"%a %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"233")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc(u"Gaana"),.country_post=tsc(u"GH"),.country_ab2=tsc(u"GH"),.country_ab3=tsc(u"GHA"),.country_num=288,.country_car=tsc(u"GH"),.country_isbn=tsc(u"9964"),.lang_name=tsc(u"Akan"),.lang_ab=tsc(u"ak"),.lang_term=tsc(u"aka")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Akan locale for Ghana"),.source=tsc(U"Sugar Labs // OLPC\t\t;\t\tfast_io"),.address=tsc(U"https://github.com/expnkx/fast_io"),.contact=tsc(U"sugarlabs.org\t\t;\t\tfast_io"),.email=tsc(U"libc-alpha@sourceware.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Akan"),.territory=tsc(U"Ghana"),.revision=tsc(U"1.0"),.date=tsc(U"2013-08-24")},.monetary={.int_curr_symbol=tsc(U"GHS "),.currency_symbol=tsc(U"GH₵"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"Kwe"),tsc(U"Dwo"),tsc(U"Ben"),tsc(U"Wuk"),tsc(U"Yaw"),tsc(U"Fia"),tsc(U"Mem")},.day={tsc(U"Kwesida"),tsc(U"Dwowda"),tsc(U"Benada"),tsc(U"Wukuda"),tsc(U"Yawda"),tsc(U"Fida"),tsc(U"Memeneda")},.abmon={tsc(U"S-Ɔ"),tsc(U"K-Ɔ"),tsc(U"E-Ɔ"),tsc(U"E-O"),tsc(U"E-K"),tsc(U"O-A"),tsc(U"A-K"),tsc(U"D-Ɔ"),tsc(U"F-Ɛ"),tsc(U"Ɔ-A"),tsc(U"Ɔ-O"),tsc(U"M-Ɔ")},.mon={tsc(U"Sanda-Ɔpɛpɔn"),tsc(U"Kwakwar-Ɔgyefuo"),tsc(U"Ebɔw-Ɔbenem"),tsc(U"Ebɔbira-Oforisuo"),tsc(U"Esusow Aketseaba-Kɔtɔnimba"),tsc(U"Obirade-Ayɛwohomumu"),tsc(U"Ayɛwoho-Kitawonsa"),tsc(U"Difuu-Ɔsandaa"),tsc(U"Fankwa-Ɛbɔ"),tsc(U"Ɔbɛsɛ-Ahinime"),tsc(U"Ɔberɛfɛw-Obubuo"),tsc(U"Mumu-Ɔpɛnimba")},.d_t_fmt=tsc(U"%a %d %b %Y %T"),.d_fmt=tsc(U"%Y//%m//%d"),.t_fmt=tsc(U"%r"),.t_fmt_ampm=tsc(U"%I:%M:%S %p"),.date_fmt=tsc(U"%a %d %b %Y %T %Z"),.am_pm={tsc(U"AN"),tsc(U"EW")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1yY]"),.noexpr=tsc(U"^[-0dDnN]"),.yesstr=tsc(U"Yiw"),.nostr=tsc(U"Daabi")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.tel_dom_fmt=tsc(U"%a %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"233")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc(U"Gaana"),.country_post=tsc(U"GH"),.country_ab2=tsc(U"GH"),.country_ab3=tsc(U"GHA"),.country_num=288,.country_car=tsc(U"GH"),.country_isbn=tsc(U"9964"),.lang_name=tsc(U"Akan"),.lang_ab=tsc(U"ak"),.lang_term=tsc(U"aka")},.measurement={.measurement=1}};


}
}

#include"../main.h"