//
//  yas_processing_channel_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_channel_tests : XCTestCase

@end

@implementation yas_processing_channel_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_channel {
    processing::channel channel;

    XCTAssertTrue(channel);
    XCTAssertEqual(channel.datas().size(), 0);
}

- (void)test_create_null {
    processing::channel channel = nullptr;

    XCTAssertFalse(channel);
}

- (void)test_insert_data {
    processing::channel channel;

    auto uint_data = make_data<uint32_t>(4);
    auto &uint_raw = get_raw<uint32_t>(uint_data);
    uint_raw[0] = 10;
    uint_raw[1] = 11;
    uint_raw[2] = 12;
    uint_raw[3] = 13;

    auto float_data = make_data<float>(2);
    auto &float_raw = get_raw<float>(float_data);
    float_raw[0] = 2.0f;
    float_raw[1] = 4.0f;

    processing::time_range uint_time_range{.start_frame = 16, .length = 4};
    processing::time_range float_time_range{.start_frame = 8, .length = 2};

    channel.insert_data(std::move(uint_time_range), std::move(uint_raw));
    channel.insert_data(std::move(float_time_range), std::move(float_data));

    std::size_t idx = 0;
    for (auto &pair : channel.datas()) {
        auto const &time_range = pair.first;
        auto const &data = pair.second;
        if (idx == 0) {
            XCTAssertTrue((time_range == processing::time_range{.start_frame = 8, .length = 2}));
            XCTAssertTrue(data.sample_type() == typeid(float));
            auto const &raw = get_raw<float>(data);
            XCTAssertEqual(raw.size(), 2);
            XCTAssertEqual(raw[0], 2.0f);
            XCTAssertEqual(raw[1], 4.0f);
        } else if (idx == 1) {
            XCTAssertTrue((time_range == processing::time_range{.start_frame = 16, .length = 4}));
            XCTAssertTrue(data.sample_type() == typeid(uint32_t));
            auto const &raw = get_raw<uint32_t>(data);
            XCTAssertEqual(raw.size(), 4);
            XCTAssertEqual(raw[0], 10);
            XCTAssertEqual(raw[1], 11);
            XCTAssertEqual(raw[2], 12);
            XCTAssertEqual(raw[3], 13);
        }

        ++idx;
    }
}

- (void)test_insert_same_time_range_data {
    processing::channel channel;
    
    auto uint_data = make_data<uint32_t>(2);
    processing::time_range uint_time_range{.start_frame = 1, .length = 2};
    
    auto float_data = make_data<float>(2);
    processing::time_range float_time_range{.start_frame = 1, .length = 2};
    
    channel.insert_data(uint_time_range, uint_data);
    channel.insert_data(float_time_range, float_data);
    
    XCTAssertEqual(channel.datas().size(), 2);
}

- (void)test_const_data {
    processing::channel channel;

    auto data = make_data<float>(1);
    get_raw<float>(data)[0] = 1.0f;

    channel.insert_data(processing::time_range{.start_frame = 10, .length = 1}, std::move(data));

    processing::channel const &const_channel = channel;

    XCTAssertEqual(const_channel.datas().size(), 1);

    for (auto const &pair : const_channel.datas()) {
        auto const &const_time_range = pair.first;
        auto const &const_data = pair.second;

        XCTAssertTrue((const_time_range == processing::time_range{.start_frame = 10, .length = 1}));
        XCTAssertEqual(get_raw<float>(const_data)[0], 1.0f);
    }
}

- (void)test_invalid_insert_data {
    processing::channel channel;
    
    auto data = make_data<float>(2);
    
    XCTAssertThrows(channel.insert_data(processing::time_range{.length = 1}, std::move(data)));
}

@end
