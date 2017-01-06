//
//  yas_processing_module_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_test_utils.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_module_tests : XCTestCase

@end

@implementation yas_processing_module_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_module {
    processing::module module{processing::module::processors_t{}};

    XCTAssertTrue(module);
    XCTAssertEqual(module.input_connectors().size(), 0);
    XCTAssertEqual(module.output_connectors().size(), 0);
}

- (void)test_create_null {
    processing::module module = nullptr;

    XCTAssertFalse(module);
}

- (void)test_process_called {
    processing::stream stream;

    processing::time called_time = nullptr;

    auto processor = [&called_time](processing::time::range const &time_range, auto const &, auto const &,
                                    processing::stream &stream) { called_time = processing::time{time_range}; };
    processing::module module{{std::move(processor)}};

    module.process({23, 456}, stream);

    XCTAssertEqual(called_time.get<time::range>().frame, 23);
    XCTAssertEqual(called_time.get<time::range>().length, 456);
}

- (void)test_connect_input {
    processing::module module{processing::module::processors_t{}};

    module.connect_input("a", 1);

    XCTAssertEqual(module.input_connectors().size(), 1);
    XCTAssertEqual(module.input_connectors().count("a"), 1);
    XCTAssertEqual(module.input_connectors().at("a").channel_index, 1);
}

- (void)test_connect_output {
    processing::module module{processing::module::processors_t{}};

    module.connect_output("b", 2);

    XCTAssertEqual(module.output_connectors().size(), 1);
    XCTAssertEqual(module.output_connectors().count("b"), 1);
    XCTAssertEqual(module.output_connectors().at("b").channel_index, 2);
}

- (void)test_disconnect_input {
    processing::module module{processing::module::processors_t{}};

    module.connect_input("c", 3);

    module.disconnect_input("c1");

    XCTAssertEqual(module.input_connectors().size(), 1);

    module.disconnect_input("c");

    XCTAssertEqual(module.input_connectors().size(), 0);
}

- (void)test_disconnect_output {
    processing::module module{processing::module::processors_t{}};

    module.connect_output("d", 3);

    module.disconnect_output("d1");

    XCTAssertEqual(module.output_connectors().size(), 1);

    module.disconnect_output("d");

    XCTAssertEqual(module.output_connectors().size(), 0);
}

@end
