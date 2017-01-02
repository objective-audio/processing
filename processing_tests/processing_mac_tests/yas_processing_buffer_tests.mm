//
//  yas_processing_buffer_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_buffer.h"
#import "yas_each_index.h"
#import <string>

using namespace yas;
using namespace yas::processing;

@interface yas_processing_buffer_tests : XCTestCase

@end

@implementation yas_processing_buffer_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_make_buffer {
    auto buffer = processing::make_buffer<float>(16);

    XCTAssertEqual(buffer.sample_byte_count(), 4);
    XCTAssertEqual(buffer.size(), 16);

    XCTAssertEqual(get_vector<float>(buffer).size(), 16);

    for (auto const &idx : make_each(16)) {
        get_vector<float>(buffer)[idx] = static_cast<float>(idx);
    }

    for (auto const &idx : make_each(16)) {
        XCTAssertEqual(get_vector<float>(buffer)[idx], static_cast<float>(idx));
    }
}

- (void)test_create_buffer_with_moved_vector {
    buffer buffer{std::vector<double>{0.0, 2.0, 4.0, 8.0}};

    XCTAssertEqual(buffer.sample_byte_count(), 8);
    XCTAssertEqual(buffer.size(), 4);

    XCTAssertEqual(get_vector<double>(buffer).size(), 4);

    XCTAssertEqual(get_vector<double>(buffer)[0], 0.0);
    XCTAssertEqual(get_vector<double>(buffer)[1], 2.0);
    XCTAssertEqual(get_vector<double>(buffer)[2], 4.0);
    XCTAssertEqual(get_vector<double>(buffer)[3], 8.0);
}

- (void)test_create_buffer_with_reference_vector {
    std::vector<int32_t> vec(3);

    buffer buffer{vec};

    vec[0] = 5;
    vec[1] = 6;
    vec[2] = 7;

    XCTAssertEqual(buffer.sample_byte_count(), 4);
    XCTAssertEqual(buffer.size(), 3);

    XCTAssertEqual(get_vector<int32_t>(buffer).size(), 3);
    XCTAssertTrue(vec.data() == get_vector<int32_t>(buffer).data());

    XCTAssertEqual(get_vector<int32_t>(buffer)[0], 5);
    XCTAssertEqual(get_vector<int32_t>(buffer)[1], 6);
    XCTAssertEqual(get_vector<int32_t>(buffer)[2], 7);
}

- (void)test_create_buffer_with_struct {
    struct element {
        std::string key;
        uint32_t value;
    };

    auto buffer = make_buffer<element>(2);
    get_vector<element>(buffer)[0].key = "zero";
    get_vector<element>(buffer)[0].value = 100;
    get_vector<element>(buffer)[1].key = "one";
    get_vector<element>(buffer)[1].value = 200;

    XCTAssertEqual(buffer.sample_byte_count(), sizeof(element));
    XCTAssertEqual(buffer.size(), 2);

    XCTAssertEqual(get_vector<element>(buffer).size(), 2);

    XCTAssertEqual(get_vector<element>(buffer)[0].key, "zero");
    XCTAssertEqual(get_vector<element>(buffer)[0].value, 100);
    XCTAssertEqual(get_vector<element>(buffer)[1].key, "one");
    XCTAssertEqual(get_vector<element>(buffer)[1].value, 200);
}

@end
