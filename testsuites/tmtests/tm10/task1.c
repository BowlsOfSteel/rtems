/*
 *
 *  COPYRIGHT (c) 1989, 1990, 1991, 1992, 1993, 1994.
 *  On-Line Applications Research Corporation (OAR).
 *  All rights assigned to U.S. Government, 1994.
 *
 *  This material may be reproduced by or for the U.S. Government pursuant
 *  to the copyright license under the clause at DFARS 252.227-7013.  This
 *  notice must appear in all copies of this file and its derivatives.
 *
 *  task1.c,v 1.2 1995/05/31 17:15:39 joel Exp
 */

#include "system.h"
#undef EXTERN
#define EXTERN
#include "conftbl.h"
#include "gvar.h"

rtems_id Queue_id;
long Buffer[4];

rtems_task High_task(
  rtems_task_argument argument
);

rtems_task Middle_tasks(
  rtems_task_argument argument
);

rtems_task Low_task(
  rtems_task_argument argument
);

void test_init();

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_status_code status;

  puts( "\n\n*** TIME TEST 10 ***" );

  test_init();

  status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of RTEMS_SELF" );
}

void test_init()
{
  rtems_unsigned32    index;
  rtems_unsigned32    size;
  rtems_task_entry    task_entry;
  rtems_status_code   status;
  rtems_task_priority priority;
  rtems_id            task_id;

  priority = 5;

  for( index = 0; index <= OPERATION_COUNT ; index++ ) {
    status = rtems_task_create(
      rtems_build_name( 'T', 'I', 'M', 'E' ),
      priority,
      1024,
      RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES,
      &task_id
    );
    directive_failed( status, "rtems_task_create LOOP" );

    priority++;

    if ( index==0 )                    task_entry = High_task;
    else if ( index==OPERATION_COUNT ) task_entry = Low_task;
    else                               task_entry = Middle_tasks;

    status = rtems_task_start( task_id, task_entry, 0 );
    directive_failed( status, "rtems_task_start LOOP" );
  }

  status = rtems_message_queue_create(
    1,
    OPERATION_COUNT,
    16,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Queue_id
  );
  directive_failed( status, "rtems_message_queue_create" );

  Timer_initialize();
    for ( index=1 ; index <= OPERATION_COUNT ; index++ )
      (void) Empty_function();
  overhead = Read_timer();

  Timer_initialize();
    for ( index=1 ; index <= OPERATION_COUNT ; index++ )
      (void) rtems_message_queue_receive(
               Queue_id,
               (long (*)[4]) Buffer,
               &size,
               RTEMS_NO_WAIT,
               RTEMS_NO_TIMEOUT
             );
  end_time = Read_timer();

  put_time(
    "rtems_message_queue_receive (RTEMS_NO_WAIT)",
    end_time,
    OPERATION_COUNT,
    overhead,
    CALLING_OVERHEAD_MESSAGE_QUEUE_RECEIVE
  );

}

rtems_task High_task(
  rtems_task_argument argument
)
{
  rtems_unsigned32 size;
  
  Timer_initialize();
     (void) rtems_message_queue_receive(
              Queue_id,
              (long (*)[4]) Buffer,
              &size,
              RTEMS_DEFAULT_OPTIONS,
              RTEMS_NO_TIMEOUT
            );
}

rtems_task Middle_tasks(
  rtems_task_argument argument
)
{
  rtems_unsigned32 size;
  
  (void) rtems_message_queue_receive(
           Queue_id,
           (long (*)[4]) Buffer,
           &size,
           RTEMS_DEFAULT_OPTIONS,
           RTEMS_NO_TIMEOUT
         );
}


rtems_task Low_task(
  rtems_task_argument argument
)
{
  end_time = Read_timer();

  put_time(
    "rtems_message_queue_receive (blocking)",
    end_time,
    OPERATION_COUNT,
    0,
    CALLING_OVERHEAD_MESSAGE_QUEUE_RECEIVE
  );

  exit( 0 );
}
