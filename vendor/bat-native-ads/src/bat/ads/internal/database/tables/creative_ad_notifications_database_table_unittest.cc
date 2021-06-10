/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/database/tables/creative_ad_notifications_database_table.h"

#include "bat/ads/internal/container_util.h"
#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

class BatAdsCreativeAdNotificationsDatabaseTableTest : public UnitTestBase {
 protected:
  BatAdsCreativeAdNotificationsDatabaseTableTest()
      : database_table_(
            std::make_unique<database::table::CreativeAdNotifications>()) {}

  ~BatAdsCreativeAdNotificationsDatabaseTableTest() override = default;

  void Save(const CreativeAdNotificationList& creative_ad_notifications) {
    database_table_->Save(creative_ad_notifications, [](const Result result) {
      ASSERT_EQ(Result::SUCCESS, result);
    });
  }

  std::unique_ptr<database::table::CreativeAdNotifications> database_table_;
};

TEST_F(BatAdsCreativeAdNotificationsDatabaseTableTest,
       SaveEmptyCreativeAdNotifications) {
  // Arrange
  CreativeAdNotificationList creative_ad_notifications = {};

  // Act
  Save(creative_ad_notifications);

  // Assert
}

TEST_F(BatAdsCreativeAdNotificationsDatabaseTableTest,
       SaveCreativeAdNotifications) {
  // Arrange
  CreativeAdNotificationList creative_ad_notifications;

  CreativeDaypartInfo daypart_info;
  CreativeAdNotificationInfo info_1;
  info_1.creative_instance_id = "3519f52c-46a4-4c48-9c2b-c264c0067f04";
  info_1.creative_set_id = "c2ba3e7d-f688-4bc4-a053-cbe7ac1e6123";
  info_1.campaign_id = "84197fc8-830a-4a8e-8339-7a70c2bfa104";
  info_1.start_at_timestamp = DistantPastAsTimestamp();
  info_1.end_at_timestamp = DistantFutureAsTimestamp();
  info_1.daily_cap = 1;
  info_1.advertiser_id = "5484a63f-eb99-4ba5-a3b0-8c25d3c0e4b2";
  info_1.priority = 2;
  info_1.per_day = 3;
  info_1.per_week = 4;
  info_1.per_month = 5;
  info_1.total_max = 6;
  info_1.segment = "Technology & Computing-Software";
  info_1.dayparts.push_back(daypart_info);
  info_1.geo_targets = {"US"};
  info_1.target_url = "https://brave.com";
  info_1.title = "Test Ad 1 Title";
  info_1.body = "Test Ad 1 Body";
  info_1.ptr = 1.0;
  creative_ad_notifications.push_back(info_1);

  CreativeAdNotificationInfo info_2;
  info_2.creative_instance_id = "eaa6224a-876d-4ef8-a384-9ac34f238631";
  info_2.creative_set_id = "184d1fdd-8e18-4baa-909c-9a3cb62cc7b1";
  info_2.campaign_id = "d1d4a649-502d-4e06-b4b8-dae11c382d26";
  info_2.start_at_timestamp = DistantPastAsTimestamp();
  info_2.end_at_timestamp = DistantFutureAsTimestamp();
  info_2.daily_cap = 1;
  info_2.advertiser_id = "8e3fac86-ce50-4409-ae29-9aa5636aa9a2";
  info_2.priority = 2;
  info_2.per_day = 3;
  info_2.per_week = 4;
  info_2.per_month = 5;
  info_2.total_max = 6;
  info_2.segment = "Technology & Computing-Software";
  info_2.dayparts.push_back(daypart_info);
  info_2.geo_targets = {"US"};
  info_2.target_url = "https://brave.com";
  info_2.title = "Test Ad 2 Title";
  info_2.body = "Test Ad 2 Body";
  info_2.ptr = 0.8;
  creative_ad_notifications.push_back(info_2);

  // Act
  Save(creative_ad_notifications);

  // Assert
  const CreativeAdNotificationList expected_creative_ad_notifications =
      creative_ad_notifications;

  const SegmentList segments = {"Technology & Computing-Software"};

  database_table_->GetForSegments(
      segments,
      [&expected_creative_ad_notifications](
          const Result result, const SegmentList& segments,
          const CreativeAdNotificationList& creative_ad_notifications) {
        EXPECT_EQ(Result::SUCCESS, result);
        EXPECT_TRUE(CompareAsSets(expected_creative_ad_notifications,
                                  creative_ad_notifications));
      });
}

TEST_F(BatAdsCreativeAdNotificationsDatabaseTableTest,
       SaveCreativeAdNotificationsInBatches) {
  // Arrange
  database_table_->set_batch_size(2);

  CreativeAdNotificationList creative_ad_notifications;

  CreativeDaypartInfo daypart_info;
  CreativeAdNotificationInfo info_1;
  info_1.creative_instance_id = "3519f52c-46a4-4c48-9c2b-c264c0067f04";
  info_1.creative_set_id = "c2ba3e7d-f688-4bc4-a053-cbe7ac1e6123";
  info_1.campaign_id = "84197fc8-830a-4a8e-8339-7a70c2bfa104";
  info_1.start_at_timestamp = DistantPastAsTimestamp();
  info_1.end_at_timestamp = DistantFutureAsTimestamp();
  info_1.daily_cap = 1;
  info_1.advertiser_id = "5484a63f-eb99-4ba5-a3b0-8c25d3c0e4b2";
  info_1.priority = 2;
  info_1.per_day = 3;
  info_1.per_week = 4;
  info_1.per_month = 5;
  info_1.total_max = 6;
  info_1.segment = "Technology & Computing-Software";
  info_1.dayparts.push_back(daypart_info);
  info_1.geo_targets = {"US"};
  info_1.target_url = "https://brave.com";
  info_1.title = "Test Ad 1 Title";
  info_1.body = "Test Ad 1 Body";
  info_1.ptr = 1.0;
  creative_ad_notifications.push_back(info_1);

  CreativeAdNotificationInfo info_2;
  info_2.creative_instance_id = "eaa6224a-876d-4ef8-a384-9ac34f238631";
  info_2.creative_set_id = "184d1fdd-8e18-4baa-909c-9a3cb62cc7b1";
  info_2.campaign_id = "d1d4a649-502d-4e06-b4b8-dae11c382d26";
  info_2.start_at_timestamp = DistantPastAsTimestamp();
  info_2.end_at_timestamp = DistantFutureAsTimestamp();
  info_2.daily_cap = 1;
  info_2.advertiser_id = "8e3fac86-ce50-4409-ae29-9aa5636aa9a2";
  info_2.priority = 2;
  info_2.per_day = 3;
  info_2.per_week = 4;
  info_2.per_month = 5;
  info_2.total_max = 6;
  info_2.segment = "Technology & Computing-Software";
  info_2.dayparts.push_back(daypart_info);
  info_2.geo_targets = {"US"};
  info_2.target_url = "https://brave.com";
  info_2.title = "Test Ad 2 Title";
  info_2.body = "Test Ad 2 Body";
  info_2.ptr = 1.0;
  creative_ad_notifications.push_back(info_2);

  CreativeAdNotificationInfo info_3;
  info_3.creative_instance_id = "a1ac44c2-675f-43e6-ab6d-500614cafe63";
  info_3.creative_set_id = "5800049f-cee5-4bcb-90c7-85246d5f5e7c";
  info_3.campaign_id = "3d62eca2-324a-4161-a0c5-7d9f29d10ab0";
  info_3.start_at_timestamp = DistantPastAsTimestamp();
  info_3.end_at_timestamp = DistantFutureAsTimestamp();
  info_3.daily_cap = 1;
  info_3.advertiser_id = "9a11b60f-e29d-4446-8d1f-318311e36e0a";
  info_3.priority = 2;
  info_3.per_day = 3;
  info_3.per_week = 4;
  info_3.per_month = 5;
  info_3.total_max = 6;
  info_3.segment = "Technology & Computing-Software";
  info_3.dayparts.push_back(daypart_info);
  info_3.geo_targets = {"US"};
  info_3.target_url = "https://brave.com";
  info_3.title = "Test Ad 3 Title";
  info_3.body = "Test Ad 3 Body";
  info_3.ptr = 1.0;
  creative_ad_notifications.push_back(info_3);

  // Act
  Save(creative_ad_notifications);

  // Assert
  const CreativeAdNotificationList expected_creative_ad_notifications =
      creative_ad_notifications;

  const SegmentList segments = {"Technology & Computing-Software"};

  database_table_->GetForSegments(
      segments,
      [&expected_creative_ad_notifications](
          const Result result, const SegmentList& segments,
          const CreativeAdNotificationList& creative_ad_notifications) {
        EXPECT_EQ(Result::SUCCESS, result);
        EXPECT_TRUE(CompareAsSets(expected_creative_ad_notifications,
                                  creative_ad_notifications));
      });
}

TEST_F(BatAdsCreativeAdNotificationsDatabaseTableTest,
       DoNotSaveDuplicateCreativeAdNotifications) {
  // Arrange
  CreativeAdNotificationList creative_ad_notifications;

  CreativeDaypartInfo daypart_info;
  CreativeAdNotificationInfo info;
  info.creative_instance_id = "3519f52c-46a4-4c48-9c2b-c264c0067f04";
  info.creative_set_id = "c2ba3e7d-f688-4bc4-a053-cbe7ac1e6123";
  info.campaign_id = "84197fc8-830a-4a8e-8339-7a70c2bfa104";
  info.start_at_timestamp = DistantPastAsTimestamp();
  info.end_at_timestamp = DistantFutureAsTimestamp();
  info.daily_cap = 1;
  info.advertiser_id = "5484a63f-eb99-4ba5-a3b0-8c25d3c0e4b2";
  info.priority = 2;
  info.per_day = 3;
  info.per_week = 4;
  info.per_month = 5;
  info.total_max = 6;
  info.segment = "Technology & Computing-Software";
  info.dayparts.push_back(daypart_info);
  info.geo_targets = {"US"};
  info.target_url = "https://brave.com";
  info.title = "Test Ad 1 Title";
  info.body = "Test Ad 1 Body";
  info.ptr = 1.0;
  creative_ad_notifications.push_back(info);

  Save(creative_ad_notifications);

  // Act
  Save(creative_ad_notifications);

  // Assert
  const CreativeAdNotificationList expected_creative_ad_notifications =
      creative_ad_notifications;

  const SegmentList segments = {"Technology & Computing-Software"};

  database_table_->GetForSegments(
      segments,
      [&expected_creative_ad_notifications](
          const Result result, const SegmentList& segments,
          const CreativeAdNotificationList& creative_ad_notifications) {
        EXPECT_EQ(Result::SUCCESS, result);
        EXPECT_TRUE(CompareAsSets(expected_creative_ad_notifications,
                                  creative_ad_notifications));
      });
}

TEST_F(BatAdsCreativeAdNotificationsDatabaseTableTest,
       GetCreativeAdNotifications) {
  // Arrange
  CreativeAdNotificationList creative_ad_notifications;

  CreativeDaypartInfo daypart_info;
  CreativeAdNotificationInfo info_1;
  info_1.creative_instance_id = "3519f52c-46a4-4c48-9c2b-c264c0067f04";
  info_1.creative_set_id = "c2ba3e7d-f688-4bc4-a053-cbe7ac1e6123";
  info_1.campaign_id = "84197fc8-830a-4a8e-8339-7a70c2bfa104";
  info_1.start_at_timestamp = DistantPastAsTimestamp();
  info_1.end_at_timestamp = DistantFutureAsTimestamp();
  info_1.daily_cap = 1;
  info_1.advertiser_id = "5484a63f-eb99-4ba5-a3b0-8c25d3c0e4b2";
  info_1.priority = 2;
  info_1.per_day = 3;
  info_1.per_week = 4;
  info_1.per_month = 5;
  info_1.total_max = 6;
  info_1.segment = "Technology & Computing-Software";
  info_1.dayparts.push_back(daypart_info);
  info_1.geo_targets = {"US"};
  info_1.target_url = "https://brave.com";
  info_1.title = "Test Ad 1 Title";
  info_1.body = "Test Ad 1 Body";
  info_1.ptr = 1.0;
  creative_ad_notifications.push_back(info_1);

  CreativeAdNotificationInfo info_2;
  info_2.creative_instance_id = "eaa6224a-876d-4ef8-a384-9ac34f238631";
  info_2.creative_set_id = "184d1fdd-8e18-4baa-909c-9a3cb62cc7b1";
  info_2.campaign_id = "d1d4a649-502d-4e06-b4b8-dae11c382d26";
  info_2.start_at_timestamp = DistantPastAsTimestamp();
  info_2.end_at_timestamp = DistantFutureAsTimestamp();
  info_2.daily_cap = 1;
  info_2.advertiser_id = "8e3fac86-ce50-4409-ae29-9aa5636aa9a2";
  info_2.priority = 2;
  info_2.per_day = 3;
  info_2.per_week = 4;
  info_2.per_month = 5;
  info_2.total_max = 6;
  info_2.segment = "Technology & Computing-Software";
  info_2.dayparts.push_back(daypart_info);
  info_2.geo_targets = {"US"};
  info_2.target_url = "https://brave.com";
  info_2.title = "Test Ad 2 Title";
  info_2.body = "Test Ad 2 Body";
  info_2.ptr = 1.0;
  creative_ad_notifications.push_back(info_2);

  Save(creative_ad_notifications);

  // Act

  // Assert
  const CreativeAdNotificationList expected_creative_ad_notifications =
      creative_ad_notifications;

  const SegmentList segments = {"Technology & Computing-Software"};

  database_table_->GetForSegments(
      segments,
      [&expected_creative_ad_notifications](
          const Result result, const SegmentList& segments,
          const CreativeAdNotificationList& creative_ad_notifications) {
        EXPECT_EQ(Result::SUCCESS, result);
        EXPECT_TRUE(CompareAsSets(expected_creative_ad_notifications,
                                  creative_ad_notifications));
      });
}

TEST_F(BatAdsCreativeAdNotificationsDatabaseTableTest,
       GetCreativeAdNotificationsForEmptySegments) {
  // Arrange
  CreativeAdNotificationList creative_ad_notifications;

  CreativeDaypartInfo daypart_info;
  CreativeAdNotificationInfo info;
  info.creative_instance_id = "3519f52c-46a4-4c48-9c2b-c264c0067f04";
  info.creative_set_id = "c2ba3e7d-f688-4bc4-a053-cbe7ac1e6123";
  info.campaign_id = "84197fc8-830a-4a8e-8339-7a70c2bfa104";
  info.start_at_timestamp = DistantPastAsTimestamp();
  info.end_at_timestamp = DistantFutureAsTimestamp();
  info.daily_cap = 1;
  info.advertiser_id = "5484a63f-eb99-4ba5-a3b0-8c25d3c0e4b2";
  info.priority = 2;
  info.per_day = 3;
  info.per_week = 4;
  info.per_month = 5;
  info.total_max = 6;
  info.segment = "Technology & Computing-Software";
  info.dayparts.push_back(daypart_info);
  info.geo_targets = {"US"};
  info.target_url = "https://brave.com";
  info.title = "Test Ad 1 Title";
  info.body = "Test Ad 1 Body";
  info.ptr = 1.0;
  creative_ad_notifications.push_back(info);

  Save(creative_ad_notifications);

  // Act

  // Assert
  const CreativeAdNotificationList expected_creative_ad_notifications = {};

  const SegmentList segments = {};

  database_table_->GetForSegments(
      segments,
      [&expected_creative_ad_notifications](
          const Result result, const SegmentList& segments,
          const CreativeAdNotificationList& creative_ad_notifications) {
        EXPECT_EQ(Result::SUCCESS, result);
        EXPECT_TRUE(CompareAsSets(expected_creative_ad_notifications,
                                  creative_ad_notifications));
      });
}

TEST_F(BatAdsCreativeAdNotificationsDatabaseTableTest,
       GetCreativeAdNotificationsForNonExistentCategory) {
  // Arrange
  CreativeAdNotificationList creative_ad_notifications;

  CreativeDaypartInfo daypart_info;
  CreativeAdNotificationInfo info;
  info.creative_instance_id = "3519f52c-46a4-4c48-9c2b-c264c0067f04";
  info.creative_set_id = "c2ba3e7d-f688-4bc4-a053-cbe7ac1e6123";
  info.campaign_id = "84197fc8-830a-4a8e-8339-7a70c2bfa104";
  info.start_at_timestamp = DistantPastAsTimestamp();
  info.end_at_timestamp = DistantFutureAsTimestamp();
  info.daily_cap = 1;
  info.advertiser_id = "5484a63f-eb99-4ba5-a3b0-8c25d3c0e4b2";
  info.priority = 2;
  info.per_day = 3;
  info.per_week = 4;
  info.per_month = 5;
  info.total_max = 6;
  info.segment = "Technology & Computing-Software";
  info.dayparts.push_back(daypart_info);
  info.geo_targets = {"US"};
  info.target_url = "https://brave.com";
  info.title = "Test Ad 1 Title";
  info.body = "Test Ad 1 Body";
  info.ptr = 1.0;
  creative_ad_notifications.push_back(info);

  Save(creative_ad_notifications);

  // Act

  // Assert
  const CreativeAdNotificationList expected_creative_ad_notifications = {};

  const SegmentList segments = {"Food & Drink"};

  database_table_->GetForSegments(
      segments,
      [&expected_creative_ad_notifications](
          const Result result, const SegmentList& segments,
          const CreativeAdNotificationList& creative_ad_notifications) {
        EXPECT_EQ(Result::SUCCESS, result);
        EXPECT_TRUE(CompareAsSets(expected_creative_ad_notifications,
                                  creative_ad_notifications));
      });
}

TEST_F(BatAdsCreativeAdNotificationsDatabaseTableTest,
       GetCreativeAdNotificationsFromMultipleSegments) {
  // Arrange
  CreativeAdNotificationList creative_ad_notifications;

  CreativeDaypartInfo daypart_info;
  CreativeAdNotificationInfo info_1;
  info_1.creative_instance_id = "3519f52c-46a4-4c48-9c2b-c264c0067f04";
  info_1.creative_set_id = "c2ba3e7d-f688-4bc4-a053-cbe7ac1e6123";
  info_1.campaign_id = "84197fc8-830a-4a8e-8339-7a70c2bfa104";
  info_1.start_at_timestamp = DistantPastAsTimestamp();
  info_1.end_at_timestamp = DistantFutureAsTimestamp();
  info_1.daily_cap = 1;
  info_1.advertiser_id = "5484a63f-eb99-4ba5-a3b0-8c25d3c0e4b2";
  info_1.priority = 2;
  info_1.per_day = 3;
  info_1.per_week = 4;
  info_1.per_month = 5;
  info_1.total_max = 6;
  info_1.segment = "Technology & Computing-Software";
  info_1.dayparts.push_back(daypart_info);
  info_1.geo_targets = {"US"};
  info_1.target_url = "https://brave.com";
  info_1.title = "Test Ad 1 Title";
  info_1.body = "Test Ad 1 Body";
  info_1.ptr = 1.0;
  creative_ad_notifications.push_back(info_1);

  CreativeAdNotificationInfo info_2;
  info_2.creative_instance_id = "eaa6224a-876d-4ef8-a384-9ac34f238631";
  info_2.creative_set_id = "184d1fdd-8e18-4baa-909c-9a3cb62cc7b1";
  info_2.campaign_id = "d1d4a649-502d-4e06-b4b8-dae11c382d26";
  info_2.start_at_timestamp = DistantPastAsTimestamp();
  info_2.end_at_timestamp = DistantFutureAsTimestamp();
  info_2.daily_cap = 1;
  info_2.advertiser_id = "8e3fac86-ce50-4409-ae29-9aa5636aa9a2";
  info_2.priority = 2;
  info_2.per_day = 3;
  info_2.per_week = 4;
  info_2.per_month = 5;
  info_2.total_max = 6;
  info_2.segment = "Food & Drink";
  info_2.dayparts.push_back(daypart_info);
  info_2.geo_targets = {"US"};
  info_2.target_url = "https://brave.com";
  info_2.title = "Test Ad 2 Title";
  info_2.body = "Test Ad 2 Body";
  info_2.ptr = 1.0;
  creative_ad_notifications.push_back(info_2);

  CreativeAdNotificationInfo info_3;
  info_3.creative_instance_id = "a1ac44c2-675f-43e6-ab6d-500614cafe63";
  info_3.creative_set_id = "5800049f-cee5-4bcb-90c7-85246d5f5e7c";
  info_3.campaign_id = "3d62eca2-324a-4161-a0c5-7d9f29d10ab0";
  info_3.start_at_timestamp = DistantPastAsTimestamp();
  info_3.end_at_timestamp = DistantFutureAsTimestamp();
  info_3.daily_cap = 1;
  info_3.advertiser_id = "9a11b60f-e29d-4446-8d1f-318311e36e0a";
  info_3.priority = 2;
  info_3.per_day = 3;
  info_3.per_week = 4;
  info_3.per_month = 5;
  info_3.total_max = 6;
  info_3.segment = "Automobiles";
  info_3.dayparts.push_back(daypart_info);
  info_3.geo_targets = {"US"};
  info_3.target_url = "https://brave.com";
  info_3.title = "Test Ad 3 Title";
  info_3.body = "Test Ad 3 Body";
  info_3.ptr = 1.0;
  creative_ad_notifications.push_back(info_3);

  Save(creative_ad_notifications);

  // Act

  // Assert
  CreativeAdNotificationList expected_creative_ad_notifications;
  expected_creative_ad_notifications.push_back(info_1);
  expected_creative_ad_notifications.push_back(info_2);

  const std::vector<std::string> segments = {"Technology & Computing-Software",
                                             "Food & Drink"};

  database_table_->GetForSegments(
      segments,
      [&expected_creative_ad_notifications](
          const Result result, const SegmentList& segments,
          const CreativeAdNotificationList& creative_ad_notifications) {
        EXPECT_EQ(Result::SUCCESS, result);
        EXPECT_TRUE(CompareAsSets(expected_creative_ad_notifications,
                                  creative_ad_notifications));
      });
}

TEST_F(BatAdsCreativeAdNotificationsDatabaseTableTest,
       GetNonExpiredCreativeAdNotifications) {
  // Arrange
  CreativeAdNotificationList creative_ad_notifications;

  CreativeDaypartInfo daypart_info;
  CreativeAdNotificationInfo info_1;
  info_1.creative_instance_id = "3519f52c-46a4-4c48-9c2b-c264c0067f04";
  info_1.creative_set_id = "c2ba3e7d-f688-4bc4-a053-cbe7ac1e6123";
  info_1.campaign_id = "84197fc8-830a-4a8e-8339-7a70c2bfa104";
  info_1.start_at_timestamp = DistantPastAsTimestamp();
  info_1.end_at_timestamp = NowAsTimestamp();
  info_1.daily_cap = 1;
  info_1.advertiser_id = "5484a63f-eb99-4ba5-a3b0-8c25d3c0e4b2";
  info_1.priority = 2;
  info_1.per_day = 3;
  info_1.per_week = 4;
  info_1.per_month = 5;
  info_1.total_max = 6;
  info_1.segment = "Technology & Computing-Software";
  info_1.dayparts.push_back(daypart_info);
  info_1.geo_targets = {"US"};
  info_1.target_url = "https://brave.com";
  info_1.title = "Test Ad 1 Title";
  info_1.body = "Test Ad 1 Body";
  info_1.ptr = 1.0;
  creative_ad_notifications.push_back(info_1);

  CreativeAdNotificationInfo info_2;
  info_2.creative_instance_id = "eaa6224a-876d-4ef8-a384-9ac34f238631";
  info_2.creative_set_id = "184d1fdd-8e18-4baa-909c-9a3cb62cc7b1";
  info_2.campaign_id = "d1d4a649-502d-4e06-b4b8-dae11c382d26";
  info_2.start_at_timestamp = DistantPastAsTimestamp();
  info_2.end_at_timestamp = DistantFutureAsTimestamp();
  info_2.daily_cap = 1;
  info_2.advertiser_id = "8e3fac86-ce50-4409-ae29-9aa5636aa9a2";
  info_2.priority = 2;
  info_2.per_day = 3;
  info_2.per_week = 4;
  info_2.per_month = 5;
  info_2.total_max = 6;
  info_2.segment = "Technology & Computing-Software";
  info_2.dayparts.push_back(daypart_info);
  info_2.geo_targets = {"US"};
  info_2.target_url = "https://brave.com";
  info_2.title = "Test Ad 2 Title";
  info_2.body = "Test Ad 2 Body";
  info_2.ptr = 1.0;
  creative_ad_notifications.push_back(info_2);

  Save(creative_ad_notifications);

  // Act
  FastForwardClockBy(base::TimeDelta::FromHours(1));

  // Assert
  CreativeAdNotificationList expected_creative_ad_notifications;
  expected_creative_ad_notifications.push_back(info_2);

  const SegmentList segments = {"Technology & Computing-Software"};

  database_table_->GetForSegments(
      segments,
      [&expected_creative_ad_notifications](
          const Result result, const SegmentList& segments,
          const CreativeAdNotificationList& creative_ad_notifications) {
        EXPECT_EQ(Result::SUCCESS, result);
        EXPECT_TRUE(CompareAsSets(expected_creative_ad_notifications,
                                  creative_ad_notifications));
      });
}

TEST_F(BatAdsCreativeAdNotificationsDatabaseTableTest,
       GetCreativeAdNotificationsMatchingCaseInsensitiveSegments) {
  // Arrange
  CreativeAdNotificationList creative_ad_notifications;

  CreativeDaypartInfo daypart_info;
  CreativeAdNotificationInfo info_1;
  info_1.creative_instance_id = "3519f52c-46a4-4c48-9c2b-c264c0067f04";
  info_1.creative_set_id = "c2ba3e7d-f688-4bc4-a053-cbe7ac1e6123";
  info_1.campaign_id = "84197fc8-830a-4a8e-8339-7a70c2bfa104";
  info_1.start_at_timestamp = DistantPastAsTimestamp();
  info_1.end_at_timestamp = DistantFutureAsTimestamp();
  info_1.daily_cap = 1;
  info_1.advertiser_id = "5484a63f-eb99-4ba5-a3b0-8c25d3c0e4b2";
  info_1.priority = 2;
  info_1.per_day = 3;
  info_1.per_week = 4;
  info_1.per_month = 5;
  info_1.total_max = 6;
  info_1.segment = "Technology & Computing-Software";
  info_1.dayparts.push_back(daypart_info);
  info_1.geo_targets = {"US"};
  info_1.target_url = "https://brave.com";
  info_1.title = "Test Ad 1 Title";
  info_1.body = "Test Ad 1 Body";
  info_1.ptr = 1.0;
  creative_ad_notifications.push_back(info_1);

  CreativeAdNotificationInfo info_2;
  info_2.creative_instance_id = "a1ac44c2-675f-43e6-ab6d-500614cafe63";
  info_2.creative_set_id = "5800049f-cee5-4bcb-90c7-85246d5f5e7c";
  info_2.campaign_id = "3d62eca2-324a-4161-a0c5-7d9f29d10ab0";
  info_2.start_at_timestamp = DistantPastAsTimestamp();
  info_2.end_at_timestamp = DistantFutureAsTimestamp();
  info_2.daily_cap = 1;
  info_2.advertiser_id = "9a11b60f-e29d-4446-8d1f-318311e36e0a";
  info_2.priority = 2;
  info_2.per_day = 3;
  info_2.per_week = 4;
  info_2.per_month = 5;
  info_2.total_max = 6;
  info_2.segment = "Food & Drink";
  info_2.dayparts.push_back(daypart_info);
  info_2.geo_targets = {"US"};
  info_2.target_url = "https://brave.com";
  info_2.title = "Test Ad 2 Title";
  info_2.body = "Test Ad 2 Body";
  info_2.ptr = 1.0;
  creative_ad_notifications.push_back(info_2);

  Save(creative_ad_notifications);

  // Act

  // Assert
  CreativeAdNotificationList expected_creative_ad_notifications;
  expected_creative_ad_notifications.push_back(info_2);

  const SegmentList segments = {"FoOd & DrInK"};

  database_table_->GetForSegments(
      segments,
      [&expected_creative_ad_notifications](
          const Result result, const SegmentList& segments,
          const CreativeAdNotificationList& creative_ad_notifications) {
        EXPECT_EQ(Result::SUCCESS, result);
        EXPECT_TRUE(CompareAsSets(expected_creative_ad_notifications,
                                  creative_ad_notifications));
      });
}

TEST_F(BatAdsCreativeAdNotificationsDatabaseTableTest, TableName) {
  // Arrange

  // Act
  const std::string table_name = database_table_->get_table_name();

  // Assert
  const std::string expected_table_name = "creative_ad_notifications";
  EXPECT_EQ(expected_table_name, table_name);
}

}  // namespace ads
