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
    XCTAssertEqual(channel.buffers().size(), 0);
}

- (void)test_create_null {
    processing::channel channel = nullptr;

    XCTAssertFalse(channel);
}

- (void)test_insert_buffer {
    processing::channel channel;

    auto uint_buffer = make_buffer<uint32_t>(4);
    auto &uint_vec = get_vector<uint32_t>(uint_buffer);
    uint_vec[0] = 10;
    uint_vec[1] = 11;
    uint_vec[2] = 12;
    uint_vec[3] = 13;

    auto float_buffer = make_buffer<float>(2);
    auto &float_vec = get_vector<float>(float_buffer);
    float_vec[0] = 2.0f;
    float_vec[1] = 4.0f;

    processing::time::range uint_time_range{.frame = 16, .length = 4};
    processing::time::range float_time_range{.frame = 8, .length = 2};

    channel.insert_buffer(std::move(uint_time_range), std::move(uint_vec));
    channel.insert_buffer(std::move(float_time_range), std::move(float_buffer));

    std::size_t idx = 0;
    for (auto &pair : channel.buffers()) {
        auto const &time_range = pair.first;
        auto const &buffer = pair.second;
        if (idx == 0) {
            XCTAssertTrue((time_range == processing::time::range{.frame = 8, .length = 2}));
            XCTAssertTrue(buffer.sample_type() == typeid(float));
            auto const &vec = get_vector<float>(buffer);
            XCTAssertEqual(vec.size(), 2);
            XCTAssertEqual(vec[0], 2.0f);
            XCTAssertEqual(vec[1], 4.0f);
        } else if (idx == 1) {
            XCTAssertTrue((time_range == processing::time::range{.frame = 16, .length = 4}));
            XCTAssertTrue(buffer.sample_type() == typeid(uint32_t));
            auto const &vec = get_vector<uint32_t>(buffer);
            XCTAssertEqual(vec.size(), 4);
            XCTAssertEqual(vec[0], 10);
            XCTAssertEqual(vec[1], 11);
            XCTAssertEqual(vec[2], 12);
            XCTAssertEqual(vec[3], 13);
        }

        ++idx;
    }
}

- (void)test_insert_same_time_range_buffer {
    processing::channel channel;

    auto uint_buffer = make_buffer<uint32_t>(2);
    processing::time::range uint_time_range{.frame = 1, .length = 2};

    auto float_buffer = make_buffer<float>(2);
    processing::time::range float_time_range{.frame = 1, .length = 2};

    channel.insert_buffer(uint_time_range, uint_buffer);
    channel.insert_buffer(float_time_range, float_buffer);

    XCTAssertEqual(channel.buffers().size(), 2);
}

- (void)test_const_buffer {
    processing::channel channel;

    auto buffer = make_buffer<float>(1);
    get_vector<float>(buffer)[0] = 1.0f;

    channel.insert_buffer(processing::time::range{.frame = 10, .length = 1}, std::move(buffer));

    processing::channel const &const_channel = channel;

    XCTAssertEqual(const_channel.buffers().size(), 1);

    for (auto const &pair : const_channel.buffers()) {
        auto const &const_time_range = pair.first;
        auto const &const_buffer = pair.second;

        XCTAssertTrue((const_time_range == processing::time::range{.frame = 10, .length = 1}));
        XCTAssertEqual(get_vector<float>(const_buffer)[0], 1.0f);
    }
}

- (void)test_invalid_insert_buffer {
    processing::channel channel;

    auto buffer = make_buffer<float>(2);

    XCTAssertThrows(channel.insert_buffer(processing::time::range{.length = 1}, std::move(buffer)));
}

@end
