//
//  yas_processing_data_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_data.h"
#import "yas_each_index.h"
#import <string>

using namespace yas;
using namespace yas::processing;

@interface yas_processing_data_tests : XCTestCase

@end

@implementation yas_processing_data_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_make_data {
    auto data = processing::make_data<float>(16);

    XCTAssertEqual(data.sample_byte_count(), 4);
    XCTAssertEqual(data.size(), 16);

    XCTAssertEqual(get_raw<float>(data).size(), 16);

    for (auto const &idx : make_each(16)) {
        get_raw<float>(data)[idx] = static_cast<float>(idx);
    }

    for (auto const &idx : make_each(16)) {
        XCTAssertEqual(get_raw<float>(data)[idx], static_cast<float>(idx));
    }
}

- (void)test_create_data_with_moved_vector {
    data data{std::vector<double>{0.0, 2.0, 4.0, 8.0}};

    XCTAssertEqual(data.sample_byte_count(), 8);
    XCTAssertEqual(data.size(), 4);

    XCTAssertEqual(get_raw<double>(data).size(), 4);

    XCTAssertEqual(get_raw<double>(data)[0], 0.0);
    XCTAssertEqual(get_raw<double>(data)[1], 2.0);
    XCTAssertEqual(get_raw<double>(data)[2], 4.0);
    XCTAssertEqual(get_raw<double>(data)[3], 8.0);
}

- (void)test_create_data_with_reference_vector {
    std::vector<int32_t> vec(3);

    data data{vec};

    vec[0] = 5;
    vec[1] = 6;
    vec[2] = 7;

    XCTAssertEqual(data.sample_byte_count(), 4);
    XCTAssertEqual(data.size(), 3);

    XCTAssertEqual(get_raw<int32_t>(data).size(), 3);
    XCTAssertTrue(vec.data() == get_raw<int32_t>(data).data());

    XCTAssertEqual(get_raw<int32_t>(data)[0], 5);
    XCTAssertEqual(get_raw<int32_t>(data)[1], 6);
    XCTAssertEqual(get_raw<int32_t>(data)[2], 7);
}

- (void)test_create_data_with_struct {
    struct element {
        std::string key;
        uint32_t value;
    };

    auto data = make_data<element>(2);
    get_raw<element>(data)[0].key = "zero";
    get_raw<element>(data)[0].value = 100;
    get_raw<element>(data)[1].key = "one";
    get_raw<element>(data)[1].value = 200;

    XCTAssertEqual(data.sample_byte_count(), sizeof(element));
    XCTAssertEqual(data.size(), 2);

    XCTAssertEqual(get_raw<element>(data).size(), 2);

    XCTAssertEqual(get_raw<element>(data)[0].key, "zero");
    XCTAssertEqual(get_raw<element>(data)[0].value, 100);
    XCTAssertEqual(get_raw<element>(data)[1].key, "one");
    XCTAssertEqual(get_raw<element>(data)[1].value, 200);
}

@end
