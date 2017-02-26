//
//  yas_processing_number_process_context_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_number_process_context.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_number_process_context_tests : XCTestCase

@end

@implementation yas_processing_number_process_context_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_by_size_2 {
    number_process_context<int8_t, 2> context;

    XCTAssertEqual(context.inputs().size(), 0);
    XCTAssertEqual(context.last_values()[0], 0);
    XCTAssertEqual(context.last_values()[1], 0);
}

- (void)test_insert_input {
    number_process_context<int8_t, 2> context;

    context.insert_input(1, 10, 0);

    XCTAssertEqual(context.inputs().size(), 1);
    XCTAssertEqual(context.inputs().begin()->first, 1);
    XCTAssertTrue(context.inputs().begin()->second.values[0]);
    XCTAssertFalse(context.inputs().begin()->second.values[1]);
    XCTAssertEqual(*context.inputs().begin()->second.values[0], 10);

    context.insert_input(1, 11, 1);

    XCTAssertEqual(context.inputs().size(), 1);
    XCTAssertEqual(context.inputs().begin()->first, 1);
    XCTAssertTrue(context.inputs().begin()->second.values[0]);
    XCTAssertTrue(context.inputs().begin()->second.values[1]);
    XCTAssertEqual(*context.inputs().begin()->second.values[0], 10);
    XCTAssertEqual(*context.inputs().begin()->second.values[1], 11);

    context.insert_input(2, 20, 1);

    XCTAssertEqual(context.inputs().size(), 2);

    auto input_iterator = context.inputs().begin();

    XCTAssertEqual((input_iterator++)->first, 1);

    XCTAssertEqual(input_iterator->first, 2);
    XCTAssertFalse(input_iterator->second.values[0]);
    XCTAssertTrue(input_iterator->second.values[1]);
    XCTAssertEqual(*(input_iterator++)->second.values[1], 20);
}

- (void)test_update_last_values {
    number_process_context<int8_t, 2> context;
    number_process_context<int8_t, 2>::input input;

    input.values[0] = 2;
    input.values[1] = std::experimental::nullopt;
    context.update_last_values(input);

    XCTAssertEqual(context.last_values()[0], 2);
    XCTAssertEqual(context.last_values()[1], 0);

    input.values[0] = std::experimental::nullopt;
    input.values[1] = 3;
    context.update_last_values(input);

    XCTAssertEqual(context.last_values()[0], 2);
    XCTAssertEqual(context.last_values()[1], 3);

    input.values[0] = 4;
    input.values[1] = 5;
    context.update_last_values(input);

    XCTAssertEqual(context.last_values()[0], 4);
    XCTAssertEqual(context.last_values()[1], 5);
}

- (void)test_reset {
    number_process_context<int8_t, 1> context;

    context.insert_input(0, 31, 0);

    number_process_context<int8_t, 1>::input input;
    input.values[0] = 32;
    context.update_last_values(input);

    context.reset();

    XCTAssertEqual(context.inputs().size(), 0);
    XCTAssertEqual(context.last_values()[0], 32);
}

@end