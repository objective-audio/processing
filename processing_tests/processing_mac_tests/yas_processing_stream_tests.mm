//
//  yas_processing_stream_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;

@interface yas_processing_stream_tests : XCTestCase

@end

@implementation yas_processing_stream_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_stream {
    processing::stream stream{{.start_frame = 123, .length = 4567}};

    XCTAssertTrue(stream);
    XCTAssertEqual(stream.time_range().start_frame, 123);
    XCTAssertEqual(stream.time_range().length, 4567);
    XCTAssertEqual(stream.channel_count(), 0);
}

- (void)test_create_null {
    processing::stream stream{nullptr};

    XCTAssertFalse(stream);
}

- (void)test_insert_channel {
    processing::stream stream{{.start_frame = 0, .length = 0}};

    stream.insert_channel(1);

    XCTAssertTrue(stream.channel_exists(1));
    XCTAssertEqual(stream.channel_count(), 1);

    stream.insert_channel(5);

    XCTAssertTrue(stream.channel_exists(5));
    XCTAssertEqual(stream.channel_count(), 2);

    stream.insert_channel(100);

    XCTAssertTrue(stream.channel_exists(100));
    XCTAssertEqual(stream.channel_count(), 3);

    stream.insert_channel(1);

    XCTAssertEqual(stream.channel_count(), 3);
    XCTAssertFalse(stream.channel_exists(0));
}

- (void)test_channel {
    processing::stream stream{{.start_frame = 0, .length = 0}};

    stream.insert_channel(2);

    auto &channel = stream.channel(2);
    channel.insert_data({}, {std::vector<int8_t>{5, 6}});

    auto const &const_stream = stream;
    auto const &const_channel = const_stream.channel(2);

    XCTAssertEqual(const_channel.datas().size(), 1);
    
    auto const &const_data = (*const_channel.datas().begin()).second;
    XCTAssertEqual(const_data.raw<int8_t>()[0], 5);
    XCTAssertEqual(const_data.raw<int8_t>()[1], 6);
}

@end
