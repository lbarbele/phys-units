#include <units/details/ratio.h>
#include <gtest/gtest.h>

#define ASSERT_SAME(...) ASSERT_TRUE((std::is_same_v<__VA_ARGS__>))

using namespace units::_details;

using ra = ratio<1, 2>;
using rb = ratio<-3, 5>;
using rc = ratio<7, -11>;
using rd = ratio<2, 4>;
using re = ratio<6, -10>;
using rf = ratio<21, -33>;
using rg = ratio<45787639226459268364595352537484737_imax, 7164295923643869682629457546_imax>;
using rh = ratio<8*rg::num, 8*rg::den>;
using ri = ratio<5>;
using rj = ratio<11>;
using rk = ratio<3131423543265623432424654325534_imax>;

TEST(rationalNumbers, integerType) {
  ASSERT_EQ(sizeof(intm_t), 16);
  ASSERT_TRUE(concepts::integral<intm_t>);

  ASSERT_EQ(gcd(0, 0), 0);
  ASSERT_EQ(gcd(30, -36, 24), 6);
  ASSERT_EQ(gcd(0, 164), 164);
  ASSERT_EQ(gcd(-53667, -25527), 201);
  ASSERT_EQ(gcd(978, 89798763754892653453379597352537489494736_imax), 6);
  ASSERT_EQ(gcd(1221, 1234567891011121314151617181920212223242526272829_imax), 3);
  ASSERT_EQ(gcd(89798763754892653453379597352537489494736_imax, 1234567891011121314151617181920212223242526272829_imax), 3);
}

TEST(rationalNumbers, prefixes) {
  ASSERT_NEAR(yocto::value<double>/1e-24 - 1, 0, 1e-14);
  ASSERT_NEAR(zepto::value<double>/1e-21 - 1, 0, 1e-14);
  ASSERT_NEAR(atto::value<double> /1e-18 - 1, 0, 1e-14);
  ASSERT_NEAR(femto::value<double>/1e-15 - 1, 0, 1e-14);
  ASSERT_NEAR(pico::value<double> /1e-12 - 1, 0, 1e-14);
  ASSERT_NEAR(nano::value<double> /1e-09 - 1, 0, 1e-14);
  ASSERT_NEAR(micro::value<double>/1e-06 - 1, 0, 1e-14);
  ASSERT_NEAR(milli::value<double>/1e-03 - 1, 0, 1e-14);
  ASSERT_NEAR(centi::value<double>/1e-02 - 1, 0, 1e-14);
  ASSERT_NEAR(deci::value<double> /1e-01 - 1, 0, 1e-14);
  ASSERT_NEAR(one::value<double>  /1e-00 - 1, 0, 1e-14);
  ASSERT_NEAR(deca::value<double> /1e+01 - 1, 0, 1e-14);
  ASSERT_NEAR(hecto::value<double>/1e+02 - 1, 0, 1e-14);
  ASSERT_NEAR(kilo::value<double> /1e+03 - 1, 0, 1e-14);
  ASSERT_NEAR(mega::value<double> /1e+06 - 1, 0, 1e-14);
  ASSERT_NEAR(giga::value<double> /1e+09 - 1, 0, 1e-14);
  ASSERT_NEAR(tera::value<double> /1e+12 - 1, 0, 1e-14);
  ASSERT_NEAR(peta::value<double> /1e+15 - 1, 0, 1e-14);
  ASSERT_NEAR(exa::value<double>  /1e+18 - 1, 0, 1e-14);
  ASSERT_NEAR(zetta::value<double>/1e+21 - 1, 0, 1e-14);
  ASSERT_NEAR(yotta::value<double>/1e+24 - 1, 0, 1e-14);
}

TEST(rationalNumbers, traitsAndConcepts) {
  ASSERT_TRUE(traits::is_ratio_v<ra>);
  ASSERT_TRUE(traits::is_ratio_v<rb>);
  ASSERT_TRUE(traits::is_ratio_v<rc>);

  ASSERT_TRUE(traits::is_reduced_ratio_v<ra>);
  ASSERT_TRUE(traits::is_reduced_ratio_v<rb>);
  ASSERT_TRUE(!traits::is_reduced_ratio_v<rc>);
  ASSERT_TRUE(!traits::is_reduced_ratio_v<rd>);
  ASSERT_TRUE(!traits::is_reduced_ratio_v<re>);
  ASSERT_TRUE(!traits::is_reduced_ratio_v<rf>);

  ASSERT_EQ(rd::num, 1);
  ASSERT_EQ(rd::den, 2);
  ASSERT_SAME(ra, typename rd::type);

  ASSERT_EQ(re::num, -3);
  ASSERT_EQ(re::den, 5);
  ASSERT_SAME(rb, typename re::type);

  ASSERT_EQ(rc::num, -7);
  ASSERT_EQ(rc::den, 11);
  ASSERT_EQ(rf::num, -7);
  ASSERT_EQ(rf::den, 11);
  ASSERT_SAME(typename rc::type, typename rf::type);

  ASSERT_TRUE(concepts::ratio<ra>);
  ASSERT_TRUE(concepts::ratio<rb>);
  ASSERT_TRUE(concepts::ratio<rc>);
  ASSERT_TRUE(concepts::ratio<rd>);
  ASSERT_TRUE(concepts::ratio<re>);
  ASSERT_TRUE(concepts::ratio<rf>);
  ASSERT_TRUE(concepts::ratio<rg>);
  ASSERT_TRUE(concepts::ratio<rh>);

  ASSERT_TRUE(concepts::reduced_ratio<ra>);
  ASSERT_TRUE(concepts::reduced_ratio<rb>);
  ASSERT_TRUE(!concepts::reduced_ratio<rc>);
  ASSERT_TRUE(!concepts::reduced_ratio<rd>);
  ASSERT_TRUE(!concepts::reduced_ratio<re>);
  ASSERT_TRUE(!concepts::reduced_ratio<rf>);
  ASSERT_TRUE(concepts::reduced_ratio<rg>);
  ASSERT_TRUE(!concepts::reduced_ratio<rh>);

  ASSERT_SAME(rg, typename rh::type);
  
  ASSERT_TRUE(concepts::integral_ratio<ri>);
  ASSERT_TRUE(concepts::integral_ratio<rj>);
  ASSERT_TRUE(concepts::integral_ratio<rk>);
  ASSERT_TRUE(!concepts::integral_ratio<ra>);
  ASSERT_TRUE(!concepts::integral_ratio<rb>);
  ASSERT_TRUE(!concepts::integral_ratio<rc>);
}

TEST(rationalNumbers, operations) {
  ASSERT_SAME(ratio_add<ra, rb>, ratio<-1, 10>);
  ASSERT_SAME(ratio_add<ra, rc>, ratio<-3, 22>);
  ASSERT_SAME(ratio_add<rb, rc>, ratio<-68, 55>);

  ASSERT_SAME(ratio_subtract<ra, rb>, ratio<11, 10>);
  ASSERT_SAME(ratio_subtract<ra, rc>, ratio<25, 22>);
  ASSERT_SAME(ratio_subtract<rb, rc>, ratio<2, 55>);

  ASSERT_SAME(ratio_multiply<ra, rb>, ratio<-3, 10>);
  ASSERT_SAME(ratio_multiply<ra, rc>, ratio<-7, 22>);
  ASSERT_SAME(ratio_multiply<rb, rc>, ratio<21, 55>);

  ASSERT_SAME(ratio_divide<ra, rb>, ratio<-5, 6>);
  ASSERT_SAME(ratio_divide<ra, rc>, ratio<-11, 14>);
  ASSERT_SAME(ratio_divide<rb, rc>, ratio<33, 35>);

  ASSERT_SAME(ratio_power<ra, 0>, one);
  ASSERT_SAME(ratio_power<rd, 0>, one);
  ASSERT_SAME(ratio_power<rg, 0>, one);
  ASSERT_SAME(ratio_power<rh, 0>, one);

  ASSERT_SAME(ratio_power<ra, 1>, typename ra::type);
  ASSERT_SAME(ratio_power<rf, 1>, typename rf::type);

  ASSERT_SAME(ratio_power<rd, 3>, ratio<1, 8>);

  ASSERT_SAME(ratio_power<rb, -1>, ratio<-5, 3>);
  ASSERT_SAME(ratio_power<rb, -2>, ratio<25, 9>);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}