//
//  yas_processing_stream_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

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
    processing::stream stream;

    XCTAssertTrue(stream);
    XCTAssertEqual(stream.channel_count(), 0);
}

- (void)test_create_null {
    processing::stream stream{nullptr};

    XCTAssertFalse(stream);
}

- (void)test_add_channel {
    processing::stream stream;

    stream.add_channel(1);

    XCTAssertTrue(stream.has_channel(1));
    XCTAssertEqual(stream.channel_count(), 1);

    stream.add_channel(5);

    XCTAssertTrue(stream.has_channel(5));
    XCTAssertEqual(stream.channel_count(), 2);

    stream.add_channel(100);

    XCTAssertTrue(stream.has_channel(100));
    XCTAssertEqual(stream.channel_count(), 3);

    stream.add_channel(1);

    XCTAssertEqual(stream.channel_count(), 3);
    XCTAssertFalse(stream.has_channel(0));
}

- (void)test_add_channel_return {
    processing::stream stream;

    auto &returned_channel = stream.add_channel(1);

    XCTAssertTrue(returned_channel);
    XCTAssertEqual(returned_channel, stream.channel(1));
}

- (void)test_channel {
    processing::stream stream;

    stream.add_channel(2);

    auto &channel = stream.channel(2);
    channel.insert_event({0, 2}, buffer{std::vector<int8_t>{5, 6}});

    auto const &const_stream = stream;
    auto const &const_channel = const_stream.channel(2);

    XCTAssertEqual(const_channel.events().size(), 1);

    auto const const_buffer = cast<processing::buffer>((*const_channel.events().begin()).second);
    XCTAssertEqual(const_buffer.vector<int8_t>()[0], 5);
    XCTAssertEqual(const_buffer.vector<int8_t>()[1], 6);
}

@end
