/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    This is an auto-generated file. Do not edit!
==============================================================================*/
namespace boost { namespace fusion
{
    struct void_;
    namespace result_of
    {
        template <
            typename K0 = void_ , typename K1 = void_ , typename K2 = void_ , typename K3 = void_ , typename K4 = void_ , typename K5 = void_ , typename K6 = void_ , typename K7 = void_ , typename K8 = void_ , typename K9 = void_
          , typename D0 = void_ , typename D1 = void_ , typename D2 = void_ , typename D3 = void_ , typename D4 = void_ , typename D5 = void_ , typename D6 = void_ , typename D7 = void_ , typename D8 = void_ , typename D9 = void_
          , typename Extra = void_
        >
        struct map_tie;
        template <>
        struct map_tie<>
        {
            typedef map<> type;
        };
    }
    BOOST_FUSION_GPU_ENABLED inline map<>
    map_tie()
    {
        return map<>();
    }
    namespace result_of
    {
        template <
            typename K0
          , typename D0
        >
        struct map_tie<K0, D0 , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_>
        {
            typedef map<fusion::pair< K0 , typename add_reference<D0>::type> > type;
        };
    }
    template <
        typename K0
      , typename D0
    >
    BOOST_FUSION_GPU_ENABLED
    inline map<fusion::pair< K0 , typename add_reference<D0>::type> >
    map_tie(D0 & _0)
    {
        return map<fusion::pair< K0 , typename add_reference<D0>::type> >(
            fusion::pair_tie<K0>(_0));
    }
    namespace result_of
    {
        template <
            typename K0 , typename K1
          , typename D0 , typename D1
        >
        struct map_tie<K0 , K1, D0 , D1 , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_>
        {
            typedef map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> > type;
        };
    }
    template <
        typename K0 , typename K1
      , typename D0 , typename D1
    >
    BOOST_FUSION_GPU_ENABLED
    inline map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> >
    map_tie(D0 & _0 , D1 & _1)
    {
        return map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> >(
            fusion::pair_tie<K0>(_0) , fusion::pair_tie<K1>(_1));
    }
    namespace result_of
    {
        template <
            typename K0 , typename K1 , typename K2
          , typename D0 , typename D1 , typename D2
        >
        struct map_tie<K0 , K1 , K2, D0 , D1 , D2 , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_>
        {
            typedef map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> > type;
        };
    }
    template <
        typename K0 , typename K1 , typename K2
      , typename D0 , typename D1 , typename D2
    >
    BOOST_FUSION_GPU_ENABLED
    inline map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> >
    map_tie(D0 & _0 , D1 & _1 , D2 & _2)
    {
        return map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> >(
            fusion::pair_tie<K0>(_0) , fusion::pair_tie<K1>(_1) , fusion::pair_tie<K2>(_2));
    }
    namespace result_of
    {
        template <
            typename K0 , typename K1 , typename K2 , typename K3
          , typename D0 , typename D1 , typename D2 , typename D3
        >
        struct map_tie<K0 , K1 , K2 , K3, D0 , D1 , D2 , D3 , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_>
        {
            typedef map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> > type;
        };
    }
    template <
        typename K0 , typename K1 , typename K2 , typename K3
      , typename D0 , typename D1 , typename D2 , typename D3
    >
    BOOST_FUSION_GPU_ENABLED
    inline map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> >
    map_tie(D0 & _0 , D1 & _1 , D2 & _2 , D3 & _3)
    {
        return map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> >(
            fusion::pair_tie<K0>(_0) , fusion::pair_tie<K1>(_1) , fusion::pair_tie<K2>(_2) , fusion::pair_tie<K3>(_3));
    }
    namespace result_of
    {
        template <
            typename K0 , typename K1 , typename K2 , typename K3 , typename K4
          , typename D0 , typename D1 , typename D2 , typename D3 , typename D4
        >
        struct map_tie<K0 , K1 , K2 , K3 , K4, D0 , D1 , D2 , D3 , D4 , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_>
        {
            typedef map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> > type;
        };
    }
    template <
        typename K0 , typename K1 , typename K2 , typename K3 , typename K4
      , typename D0 , typename D1 , typename D2 , typename D3 , typename D4
    >
    BOOST_FUSION_GPU_ENABLED
    inline map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> >
    map_tie(D0 & _0 , D1 & _1 , D2 & _2 , D3 & _3 , D4 & _4)
    {
        return map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> >(
            fusion::pair_tie<K0>(_0) , fusion::pair_tie<K1>(_1) , fusion::pair_tie<K2>(_2) , fusion::pair_tie<K3>(_3) , fusion::pair_tie<K4>(_4));
    }
    namespace result_of
    {
        template <
            typename K0 , typename K1 , typename K2 , typename K3 , typename K4 , typename K5
          , typename D0 , typename D1 , typename D2 , typename D3 , typename D4 , typename D5
        >
        struct map_tie<K0 , K1 , K2 , K3 , K4 , K5, D0 , D1 , D2 , D3 , D4 , D5 , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_ , void_>
        {
            typedef map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> > type;
        };
    }
    template <
        typename K0 , typename K1 , typename K2 , typename K3 , typename K4 , typename K5
      , typename D0 , typename D1 , typename D2 , typename D3 , typename D4 , typename D5
    >
    BOOST_FUSION_GPU_ENABLED
    inline map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> >
    map_tie(D0 & _0 , D1 & _1 , D2 & _2 , D3 & _3 , D4 & _4 , D5 & _5)
    {
        return map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> >(
            fusion::pair_tie<K0>(_0) , fusion::pair_tie<K1>(_1) , fusion::pair_tie<K2>(_2) , fusion::pair_tie<K3>(_3) , fusion::pair_tie<K4>(_4) , fusion::pair_tie<K5>(_5));
    }
    namespace result_of
    {
        template <
            typename K0 , typename K1 , typename K2 , typename K3 , typename K4 , typename K5 , typename K6
          , typename D0 , typename D1 , typename D2 , typename D3 , typename D4 , typename D5 , typename D6
        >
        struct map_tie<K0 , K1 , K2 , K3 , K4 , K5 , K6, D0 , D1 , D2 , D3 , D4 , D5 , D6 , void_ , void_ , void_ , void_ , void_ , void_ , void_>
        {
            typedef map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> , fusion::pair< K6 , typename add_reference<D6>::type> > type;
        };
    }
    template <
        typename K0 , typename K1 , typename K2 , typename K3 , typename K4 , typename K5 , typename K6
      , typename D0 , typename D1 , typename D2 , typename D3 , typename D4 , typename D5 , typename D6
    >
    BOOST_FUSION_GPU_ENABLED
    inline map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> , fusion::pair< K6 , typename add_reference<D6>::type> >
    map_tie(D0 & _0 , D1 & _1 , D2 & _2 , D3 & _3 , D4 & _4 , D5 & _5 , D6 & _6)
    {
        return map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> , fusion::pair< K6 , typename add_reference<D6>::type> >(
            fusion::pair_tie<K0>(_0) , fusion::pair_tie<K1>(_1) , fusion::pair_tie<K2>(_2) , fusion::pair_tie<K3>(_3) , fusion::pair_tie<K4>(_4) , fusion::pair_tie<K5>(_5) , fusion::pair_tie<K6>(_6));
    }
    namespace result_of
    {
        template <
            typename K0 , typename K1 , typename K2 , typename K3 , typename K4 , typename K5 , typename K6 , typename K7
          , typename D0 , typename D1 , typename D2 , typename D3 , typename D4 , typename D5 , typename D6 , typename D7
        >
        struct map_tie<K0 , K1 , K2 , K3 , K4 , K5 , K6 , K7, D0 , D1 , D2 , D3 , D4 , D5 , D6 , D7 , void_ , void_ , void_ , void_ , void_>
        {
            typedef map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> , fusion::pair< K6 , typename add_reference<D6>::type> , fusion::pair< K7 , typename add_reference<D7>::type> > type;
        };
    }
    template <
        typename K0 , typename K1 , typename K2 , typename K3 , typename K4 , typename K5 , typename K6 , typename K7
      , typename D0 , typename D1 , typename D2 , typename D3 , typename D4 , typename D5 , typename D6 , typename D7
    >
    BOOST_FUSION_GPU_ENABLED
    inline map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> , fusion::pair< K6 , typename add_reference<D6>::type> , fusion::pair< K7 , typename add_reference<D7>::type> >
    map_tie(D0 & _0 , D1 & _1 , D2 & _2 , D3 & _3 , D4 & _4 , D5 & _5 , D6 & _6 , D7 & _7)
    {
        return map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> , fusion::pair< K6 , typename add_reference<D6>::type> , fusion::pair< K7 , typename add_reference<D7>::type> >(
            fusion::pair_tie<K0>(_0) , fusion::pair_tie<K1>(_1) , fusion::pair_tie<K2>(_2) , fusion::pair_tie<K3>(_3) , fusion::pair_tie<K4>(_4) , fusion::pair_tie<K5>(_5) , fusion::pair_tie<K6>(_6) , fusion::pair_tie<K7>(_7));
    }
    namespace result_of
    {
        template <
            typename K0 , typename K1 , typename K2 , typename K3 , typename K4 , typename K5 , typename K6 , typename K7 , typename K8
          , typename D0 , typename D1 , typename D2 , typename D3 , typename D4 , typename D5 , typename D6 , typename D7 , typename D8
        >
        struct map_tie<K0 , K1 , K2 , K3 , K4 , K5 , K6 , K7 , K8, D0 , D1 , D2 , D3 , D4 , D5 , D6 , D7 , D8 , void_ , void_ , void_>
        {
            typedef map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> , fusion::pair< K6 , typename add_reference<D6>::type> , fusion::pair< K7 , typename add_reference<D7>::type> , fusion::pair< K8 , typename add_reference<D8>::type> > type;
        };
    }
    template <
        typename K0 , typename K1 , typename K2 , typename K3 , typename K4 , typename K5 , typename K6 , typename K7 , typename K8
      , typename D0 , typename D1 , typename D2 , typename D3 , typename D4 , typename D5 , typename D6 , typename D7 , typename D8
    >
    BOOST_FUSION_GPU_ENABLED
    inline map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> , fusion::pair< K6 , typename add_reference<D6>::type> , fusion::pair< K7 , typename add_reference<D7>::type> , fusion::pair< K8 , typename add_reference<D8>::type> >
    map_tie(D0 & _0 , D1 & _1 , D2 & _2 , D3 & _3 , D4 & _4 , D5 & _5 , D6 & _6 , D7 & _7 , D8 & _8)
    {
        return map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> , fusion::pair< K6 , typename add_reference<D6>::type> , fusion::pair< K7 , typename add_reference<D7>::type> , fusion::pair< K8 , typename add_reference<D8>::type> >(
            fusion::pair_tie<K0>(_0) , fusion::pair_tie<K1>(_1) , fusion::pair_tie<K2>(_2) , fusion::pair_tie<K3>(_3) , fusion::pair_tie<K4>(_4) , fusion::pair_tie<K5>(_5) , fusion::pair_tie<K6>(_6) , fusion::pair_tie<K7>(_7) , fusion::pair_tie<K8>(_8));
    }
    namespace result_of
    {
        template <
            typename K0 , typename K1 , typename K2 , typename K3 , typename K4 , typename K5 , typename K6 , typename K7 , typename K8 , typename K9
          , typename D0 , typename D1 , typename D2 , typename D3 , typename D4 , typename D5 , typename D6 , typename D7 , typename D8 , typename D9
        >
        struct map_tie<K0 , K1 , K2 , K3 , K4 , K5 , K6 , K7 , K8 , K9, D0 , D1 , D2 , D3 , D4 , D5 , D6 , D7 , D8 , D9 , void_>
        {
            typedef map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> , fusion::pair< K6 , typename add_reference<D6>::type> , fusion::pair< K7 , typename add_reference<D7>::type> , fusion::pair< K8 , typename add_reference<D8>::type> , fusion::pair< K9 , typename add_reference<D9>::type> > type;
        };
    }
    template <
        typename K0 , typename K1 , typename K2 , typename K3 , typename K4 , typename K5 , typename K6 , typename K7 , typename K8 , typename K9
      , typename D0 , typename D1 , typename D2 , typename D3 , typename D4 , typename D5 , typename D6 , typename D7 , typename D8 , typename D9
    >
    BOOST_FUSION_GPU_ENABLED
    inline map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> , fusion::pair< K6 , typename add_reference<D6>::type> , fusion::pair< K7 , typename add_reference<D7>::type> , fusion::pair< K8 , typename add_reference<D8>::type> , fusion::pair< K9 , typename add_reference<D9>::type> >
    map_tie(D0 & _0 , D1 & _1 , D2 & _2 , D3 & _3 , D4 & _4 , D5 & _5 , D6 & _6 , D7 & _7 , D8 & _8 , D9 & _9)
    {
        return map<fusion::pair< K0 , typename add_reference<D0>::type> , fusion::pair< K1 , typename add_reference<D1>::type> , fusion::pair< K2 , typename add_reference<D2>::type> , fusion::pair< K3 , typename add_reference<D3>::type> , fusion::pair< K4 , typename add_reference<D4>::type> , fusion::pair< K5 , typename add_reference<D5>::type> , fusion::pair< K6 , typename add_reference<D6>::type> , fusion::pair< K7 , typename add_reference<D7>::type> , fusion::pair< K8 , typename add_reference<D8>::type> , fusion::pair< K9 , typename add_reference<D9>::type> >(
            fusion::pair_tie<K0>(_0) , fusion::pair_tie<K1>(_1) , fusion::pair_tie<K2>(_2) , fusion::pair_tie<K3>(_3) , fusion::pair_tie<K4>(_4) , fusion::pair_tie<K5>(_5) , fusion::pair_tie<K6>(_6) , fusion::pair_tie<K7>(_7) , fusion::pair_tie<K8>(_8) , fusion::pair_tie<K9>(_9));
    }
}}
