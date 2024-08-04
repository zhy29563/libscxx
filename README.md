# Google Logging Library

Google Logging (`glog`) is a C++14 library that implements application-level logging. The library provides logging `APIs` based on C++-style streams and various helper macros.

## How to Use

You can log a message by simply streaming things to `LOG`(<a particular `severity level`>), e.g.,

```cpp
#include <glog/logging.h>

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]); 
    LOG(INFO) << "Found " << num_cookies << " cookies"; 
}
```

## User Guide

### Logging

`glog` defines a series of macros that simplify many common logging tasks. You can log messages by:

- `severity level`
- `control logging` behavior from the command line
- log based on `conditionals`
- abort the program when `expected conditions` are not met
- introduce your `own logging levels`
- `customize the prefix` attached to log messages
- more

#### Severity Levels

You can specify one of the following severity levels (in increasing order of severity):

1. `INFO`
2. `WARNING`
3. `ERROR`
4. `FATAL`

Logging a `FATAL` message terminates the program (after the message is logged).

> **Note**
>
> Messages of a given severity are logged not only to corresponding severity log file but also to other log files of lower severity. For instance, a message of severity `FATAL` will be logged to log files of severity `FATAL`, `ERROR`, `WARNING`, and `INFO`.

The `DFATAL` severity logs a `FATAL` error in debug mode (i.e., there is no `NDEBUG` macro defined), but avoids halting the program in production by automatically reducing the severity to `ERROR`.

#### Log Files

Unless otherwise specified, `glog` uses the format

```
<tmp>/<program name>.<hostname>.<user name>.log.<severity level>.<date>-<time>.<pid>
```

for log filenames written to a directory designated as `<tmp>` and determined according to the following rules.

- **Windows**

  `glog` uses the [GetTempPathA](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-gettemppatha) API function to retrieve the directory for temporary files with a fallback to

  1. `C:\TMP\`
  2. `C:\TEMP\`

  (in the order given.)

- **non-Windows**

  The directory is determined by referencing the environment variables

  1. `TMPDIR`
  2. `TMP`

  if set with a fallback to `/tmp/`.

The default path to a log file on Linux, for instance, could be

```
/tmp/hello_world.example.com.hamaji.log.INFO.20080709-222411.10474
```

> **Note**
>
> By default, glog echos `ERROR` and `FATAL` messages to standard error in addition to log files.

#### Log Line Prefix Format

Log lines have this form:

```
Lyyyymmdd hh:mm:ss.uuuuuu threadid file:line] msg...
```

where the fields are defined as follows:

| Placeholder       | Meaning                                                      |
| ----------------- | ------------------------------------------------------------ |
| `L`               | A single character, representing the log level (e.g., `I` for `INFO`) |
| `yyyy`            | The year                                                     |
| `mm`              | The month (zero padded; i.e., May is `05`)                   |
| `dd`              | The day (zero padded)                                        |
| `hh:mm:ss.uuuuuu` | Time in hours, minutes and fractional seconds                |
| `threadid`        | The space-padded thread ID                                   |
| `file`            | The file name                                                |
| `line`            | The line number                                              |
| `msg`             | The user-supplied message                                    |

> **Default log line prefix format**
>
> ```
> I1103 11:57:31.739339 24395 google.cc:2341] Command line: ./some_prog
> I1103 11:57:31.739403 24395 google.cc:2342] Process id 24395
> ```

> **Note**
>
> ```
> Although microseconds are useful for comparing events on a single machine, clocks on different machines may not be well synchronized. Hence, use with caution when comparing the low bits of timestamps from different machines.
> ```

#### Format Customization

The predefined log line prefix can be replaced using a user-provided callback that formats the corresponding output.

For each log entry, the callback will be invoked with a reference to a `google::LogMessage` instance containing the severity, filename, line number, thread ID, and time of the event. It will also be given a reference to the output stream, whose contents will be prepended to the actual message in the final log line.

To enable the use of a prefix formatter, use the

```cpp
google::InstallPrefixFormatter(&MyPrefixFormatter);
```

function to pass a pointer to the corresponding `MyPrefixFormatter` callback during initialization. `InstallPrefixFormatter` takes a second optional argument of type `void*` that allows supplying user data to the callback.

> **Custom prefix formatter**
>
> The following function outputs a prefix that matches glog's default format. The third parameter `data` can be used to access user-supplied data which unless specified defaults to `nullptr`.
>
> ```cpp
> void MyPrefixFormatter(std::ostream& s, const google::LogMessage& m, void* /*data*/) {
>    s << google::GetLogSeverityName(m.severity())[0]
>    << setw(4) << 1900 + m.time().year()
>    << setw(2) << 1 + m.time().month()
>    << setw(2) << m.time().day()
>    << ' '
>    << setw(2) << m.time().hour() << ':'
>    << setw(2) << m.time().min()  << ':'
>    << setw(2) << m.time().sec() << "."
>    << setw(6) << m.time().usec()
>    << ' '
>    << setfill(' ') << setw(5)
>    << m.thread_id() << setfill('0')
>    << ' '
>    << m.basename() << ':' << m.line() << "]";
> }
> ```

#### Conditional / Occasional Logging

Sometimes, you may only want to log a message under certain conditions. You can use the following macros to perform conditional logging:

```cpp
LOG_IF(INFO, num_cookies > 10) << "Got lots of cookies";
```

The "Got lots of cookies" message is logged only when the variable `num_cookies` exceeds 10. If a line of code is executed many times, it may be useful to only log a message at certain intervals. This kind of logging is most useful for informational messages.

```cpp
LOG_EVERY_N(INFO, 10) << "Got the " << google::COUNTER << "th cookie";
```

The above line outputs a log messages on the 1st, 11th, 21st, ... times it is executed.

> **Note**
>
> The placeholder `google::COUNTER` identifies the recurring repetition.

You can combine conditional and occasional logging with the following macro.

```cpp
LOG_IF_EVERY_N(INFO, (size > 1024), 10) << "Got the " << google::COUNTER << "th big cookie";
```

Instead of outputting a message every nth time, you can also limit the output to the first n occurrences:

```cpp
LOG_FIRST_N(INFO, 20) << "Got the " << google::COUNTER << "th cookie";
```

Outputs log messages for the first 20 times it is executed. The `google::COUNTER` identifier indicates which repetition is happening.

Other times, it is desired to only log a message periodically based on a time. For instance, to log a message every 10ms:

```cpp
LOG_EVERY_T(INFO, 0.01) << "Got a cookie";
```

Or every 2.35s:

```cpp
LOG_EVERY_T(INFO, 2.35) << "Got a cookie";
```

#### Verbose Logging

When you are chasing difficult bugs, thorough log messages are very useful. However, you may want to ignore too verbose messages in usual development. For such verbose logging, glog provides the `VLOG` macro, which allows you to define your own numeric logging levels.

The `--v` command line option controls which verbose messages are logged:

```cpp
VLOG(1) << "I’m printed when you run the program with --v=1 or higher";
VLOG(2) << "I’m printed when you run the program with --v=2 or higher";
```

With `VLOG`, the lower the verbose level, the more likely messages are to be logged. For example, if `--v==1`, `VLOG(1)` will log, but `VLOG(2)` will not log.

> **Warning**
>
> The `VLOG` behavior is opposite of the severity level logging, where `INFO`, `ERROR`, etc. are defined in increasing order and thus `--minloglevel` of 1 will only log `WARNING` and above.

Though you can specify any integers for both `VLOG` macro and `--v` flag, the common values for them are small positive integers. For example, if you write `VLOG(0)`, you should specify `--v=-1` or lower to silence it. This is less useful since we may not want verbose logs by default in most cases. The `VLOG` macros always log at the `INFO` log level (when they log at all).

Verbose logging can be controlled from the command line on a per-module basis:

```
--vmodule=mapreduce=2,file=1,gfs*=3 --v=0
```

Specifying these options will specifically:

1. Print `VLOG(2)` and lower messages from mapreduce.{h,cc}
2. Print `VLOG(1)` and lower messages from file.{h,cc}
3. Print `VLOG(3)` and lower messages from files prefixed with "gfs"
4. Print `VLOG(0)` and lower messages from elsewhere

The wildcarding functionality 3. supports both `*` (matches 0 or more characters) and `?` (matches any single character) wildcards. Please also refer to [command line flags](https://google.github.io/glog/stable/flags/) for more information.

There's also `VLOG_IS_ON(n)` "verbose level" condition macro. This macro returns `true` when the `--v` is equal to or greater than `n`. The macro can be used as follows:

```cpp
if (VLOG_IS_ON(2)) {
    // 
}
```

Verbose level condition macros `VLOG_IF`, `VLOG_EVERY_N` and `VLOG_IF_EVERY_N` behave analogous to `LOG_IF`, `LOG_EVERY_N`, `LOG_IF_EVERY_N`, but accept a numeric verbosity level as opposed to a severity level.

```cpp
VLOG_IF(1, (size > 1024)) << "I’m printed when size is more than 1024 and when you run the program with --v=1 or more";
VLOG_EVERY_N(1, 10) << "I’m printed every 10th occurrence, and when you run the program with --v=1 or more. Present occurrence is " << google::COUNTER;
VLOG_IF_EVERY_N(1, (size > 1024), 10) << "I’m printed on every 10th occurrence of case when size is more  than 1024, when you run the program with --v=1 or more. Present occurrence is " << google::COUNTER;
```

> **Performance**
>
> The conditional logging macros provided by glog (e.g., `CHECK`, `LOG_IF`, `VLOG`, etc.) are carefully implemented and don't execute the right hand side expressions when the conditions are false. So, the following check may not sacrifice the performance of your application.
>
> ```cpp
> CHECK(obj.ok) << obj.CreatePrettyFormattedStringButVerySlow();
> ```

#### Debugging Support

Special debug mode logging macros only have an effect in debug mode and are compiled away to nothing for non-debug mode compiles. Use these macros to avoid slowing down your production application due to excessive logging.

```cpp
DLOG(INFO) << "Found cookies";
DLOG_IF(INFO, num_cookies > 10) << "Got lots of cookies";
DLOG_EVERY_N(INFO, 10) << "Got the " << google::COUNTER << "th cookie";
DLOG_FIRST_N(INFO, 10) << "Got the " << google::COUNTER << "th cookie";
DLOG_EVERY_T(INFO, 0.01) << "Got a cookie";
```

#### Runtime Checks

It is a good practice to check expected conditions in your program frequently to detect errors as early as possible. The `CHECK` macro provides the ability to `abort the application` when a `condition is not met`, similar to the `assert` macro defined in the standard C library.

`CHECK` aborts the application if a `condition is not true`. Unlike `assert`, it is **not** controlled by `NDEBUG`, so the check will be executed regardless of compilation mode. Therefore, `fp->Write(x)` in the following example is always executed:

```cpp
CHECK(fp->Write(x) == 4) << "Write failed!";
```

There are various helper macros for equality/inequality checks -`CHECK_EQ`, `CHECK_NE`, `CHECK_LE`, `CHECK_LT`, `CHECK_GE`, and `CHECK_GT`. They compare two values, and log a `FATAL` message `including the two values when the result is not as expected`. The values must have `operator<<(ostream, ...)` defined.

You may append to the error message like so:

```cpp
CHECK_NE(1, 2) << ": The world must be ending!";
```

We are very careful to ensure that each argument is evaluated exactly once, and that anything which is legal to pass as a function argument is legal here. In particular, the arguments may be temporary expressions which will end up being destroyed at the end of the apparent statement, for example:

```cpp
CHECK_EQ(string("abc")[1], ’b’);
```

The compiler reports an error if one of the arguments is a pointer and the other is `nullptr`. To work around this, simply `static_cast` `nullptr` to the type of the desired pointer.

```cpp
CHECK_EQ(some_ptr, static_cast<SomeType*>(nullptr));
```

Better yet, use the `CHECK_NOTNULL` macro:

```cpp
CHECK_NOTNULL(some_ptr);
some_ptr->DoSomething();
```

Since this macro returns the given pointer, this is very useful in constructor initializer lists.

```cpp
struct S {
    S(Something* ptr) : ptr_(CHECK_NOTNULL(ptr)) {}
    Something* ptr_;
};
```

> **Warning**
>
> Due to the argument forwarding, `CHECK_NOTNULL` cannot be used to simultaneously stream an additional custom message. To provide a custom message, one can use the macro `CHECK_EQ` prior to the failing check.

If you are comparing C strings (`char *`), a handy set of macros performs both case sensitive and insensitive comparisons - `CHECK_STREQ`, `CHECK_STRNE`, `CHECK_STRCASEEQ`, and `CHECK_STRCASENE`. The `CHECK_*CASE*` macro variants are case-insensitive. You can safely pass `nullptr` pointers to this macro. They treat `nullptr` and any non-`nullptr` string as not equal. Two `nullptr`s are equal.

> **Note**
>
> Both arguments may be temporary objects which are destructed at the end of the current *full expression*, such as
>
> ```cpp
> CHECK_STREQ(Foo().c_str(), Bar().c_str());
> ```
>
> where `Foo` and `Bar` return `std::string`.

The `CHECK_DOUBLE_EQ` macro checks the equality of two floating point values, accepting a small error margin. `CHECK_NEAR` accepts a third floating point argument, which specifies the acceptable error margin.

#### Raw Logging

The header file `<glog/raw_logging.h>` can be used for thread-safe logging, which does not allocate any memory or acquire any locks. Therefore, the macros defined in this header file can be used by low-level memory allocation and synchronization code. Please check [src/glog/raw_logging.h](https://github.com/google/glog/blob/0.7.x/src/glog/raw_logging.h) for detail.

#### Google Style `perror()`

`PLOG()` and `PLOG_IF()` and `PCHECK()` behave exactly like their `LOG*` and `CHECK` equivalents with the addition that they append a description of the current state of `errno` to their output lines. E.g.

```cpp
PCHECK(write(1, nullptr, 2) >= 0) << "Write nullptr failed";
```

This check fails with the following error message.

```cpp
F0825 185142 test.cc:22] Check failed: write(1, nullptr, 2) >= 0 Write nullptr failed: Bad address [14]
```

#### Syslog

`SYSLOG`, `SYSLOG_IF`, and `SYSLOG_EVERY_N` macros are available. These log to syslog in addition to the normal logs. Be aware that logging to syslog can drastically impact performance, especially if syslog is configured for remote logging! Make sure you understand the implications of outputting to syslog before you use these macros. In general, it's wise to use these macros sparingly.

### Adjusting Output

Several flags influence glog's output behavior.

#### Using Command-line Parameters and Environment Variables

If the `Google gflags library` is installed on your machine, the build system will automatically detect and use it, allowing you to pass flags on the command line.

> **Activate** `--logtostderr` **in an application from the command line**
>
> A binary `you_application` that uses glog can be started using
>
> ```
> ./your_application --logtostderr=1
> ```
>
> to log to `stderr` instead of writing the output to a log file.

> **Tip**
>
> You can set boolean flags to `true` by specifying `1`, `true`, or `yes`. To set boolean flags to `false`, specify `0`, `false`, or `no`. In either case the spelling is case-insensitive.

If the Google gflags library isn't installed, you set flags via environment variables, prefixing the flag name with `GLOG_`, e.g.,

> **Activate** `logtostderr` **without gflags**
>
> ```
> GLOG_logtostderr=1 ./your_application
> ```

The following flags are most commonly used:

- `logtostderr` (`bool`, default=`false`)

  Log messages to `stderr` instead of logfiles.

- `stderrthreshold` (`int`, default=2, which is `ERROR`)

  Copy log messages at or above this level to `stderr` in addition to logfiles. The numbers of severity levels `INFO`, `WARNING`, `ERROR`, and `FATAL` are 0, 1, 2, and 3, respectively.

- `minloglevel` (`int`, default=0, which is `INFO`)

  Log messages at or above this level. Again, the numbers of severity levels `INFO`, `WARNING`, `ERROR`, and `FATAL` are 0, 1, 2, and 3, respectively.

- `log_dir` (`string`, default="")

  If specified, logfiles are written into this directory instead of the default logging directory.

- `v` (`int`, default=0)

  Show all `VLOG(m)` messages for `m` less or equal the value of this flag. Overridable by `--vmodule`. 

- `vmodule` (`string`, default="")

  Per-module verbose level. The argument has to contain a comma-separated list of `<module name>=<log level>`. `<module name>` is a glob pattern (e.g., `gfs*` for all modules whose name starts with "gfs"), matched against the filename base (that is, name ignoring .cc/.h./-inl.h). `<log level>` overrides any value given by `--v`. 

Additional flags are defined in [flags.cc](https://github.com/google/glog/blob/0.7.x/src/flags.cc). Please see the source for their complete list.

#### Modifying Flags Programmatically

You can also modify flag values in your program by modifying global variables `FLAGS_*`. Most settings start working immediately after you update `FLAGS_*`. The exceptions are the flags related to destination files. For instance, you might want to set `FLAGS_log_dir` before calling `google::InitGoogleLogging`.

> Setting `log_dir` at runtime
>
> ```
> LOG(INFO) << "file";
> // Most flags work immediately after updating values.
> FLAGS_logtostderr = 1;
> LOG(INFO) << "stderr";
> FLAGS_logtostderr = 0;
> // This won’t change the log destination. If you want to set this
> // value, you should do this before google::InitGoogleLogging .
> FLAGS_log_dir = "/some/log/directory";
> LOG(INFO) << "the same file";
> ```

### Custom Sinks

Under certain circumstances, it is useful to send the log output to a destination other than a file, `stderr` and/or `stdout`. In case, the library provides the `google::LogSink` interface whose implementations can be used to write the log output to arbitrary locations.

#### Basic Interface

The sink interface is defined as follows:

```cpp
class LogSink {
 public:
  virtual void send(LogSeverity severity, const char* full_filename,
                    const char* base_filename, int line,
                    const LogMessageTime& time, const char* message,
                    size_t message_len);
};
```

The user must implement `google::LogSink::send`, which is called by the library every time a message is logged.

Possible deadlock due to nested logging

This method can't use `LOG()` or `CHECK()` as logging system mutex(s) are held during this call.

#### Registering Log Sinks

To use the custom sink and instance of the above interface implementation must be registered using `google::AddLogSink` which expects a pointer to the `google::LogSink` instance. To unregister use `google::RemoveLogSink`. Both functions are thread-safe.

> **`LogSink` ownership**
>
> The `google::LogSink` instance must not be destroyed until the referencing pointer is unregistered.

#### Direct Logging

Instead of registering the sink, we can directly use to log messages. While `LOG_TO_SINK(sink, severity)` allows to log both to the sink and to a global log registry, e.g., a file, `LOG_TO_SINK_BUT_NOT_TO_LOGFILE(sink, severity)` will avoid the latter.

>**Using a custom sink**
>
>```cpp
>#include <glog/logging.h>
>
>#include <algorithm>
>#include <cstddef>
>#include <iostream>
>#include <iterator>
>
>namespace {
>
>struct MyLogSink : google::LogSink {  
>  void send(google::LogSeverity severity, const char* /*full_filename*/,
>            const char* base_filename, int line,
>            const google::LogMessageTime& /*time*/, const char* message,
>            std::size_t message_len) override {
>    std::cout << google::GetLogSeverityName(severity) << ' ' << base_filename
>              << ':' << line << ' ';
>    std::copy_n(message, message_len,
>                std::ostreambuf_iterator<char>{std::cout});
>    std::cout << '\n';
>  }
>};
>
>}  // namespace
>
>int main(int /*argc*/, char** argv) {
>  google::InitGoogleLogging(argv[0]);
>
>  MyLogSink sink;
>  google::AddLogSink(&sink);  
>
>  LOG(INFO) << "logging to MySink";
>
>  google::RemoveLogSink(&sink);  
>
>  // We can directly log to a sink without registering it
>  LOG_TO_SINK(&sink, INFO) << "direct logging";  
>  LOG_TO_SINK_BUT_NOT_TO_LOGFILE(&sink, INFO)
>      << "direct logging but not to file";
>}
>
>```
>
>Running the above example as `GLOG_log_dir=. ./custom_sink_example` will produce
>
>> **Custom sink output**
>>
>> ```
>> INFO custom_sink.cc:63 logging to MySink
>> INFO custom_sink.cc:68 direct logging
>> INFO custom_sink.cc:69 direct logging but not to file 
>> ```
>>
>
>and the corresponding log file will contain
>
>**Log file generated with the custom sink**
>
>```
>Log file created at: 2024/06/11 13:24:27
>Running on machine: pc
>Running duration (h:mm:ss): 0:00:00
>Log line format: [IWEF]yyyymmdd hh:mm:ss.uuuuuu threadid file:line] msg
>I20240611 13:24:27.476620 126237946035776 custom_sink.cc:63] logging to MySink
>I20240611 13:24:27.476796 126237946035776 custom_sink.cc:68] direct logging
>```

### Failure Signal Handler

#### Stacktrace as Default Failure Handler

The library provides a convenient signal handler that will dump useful information when the program crashes on certain signals such as `SIGSEGV`. The signal handler can be installed by `google::InstallFailureSignalHandler()`. The following is an example of output from the signal handler.

```
*** Aborted at 1225095260 (unix time) try "date -d @1225095260" if you are using GNU date ***
*** SIGSEGV (@0x0) received by PID 17711 (TID 0x7f893090a6f0) from PID 0; stack trace: ***
PC: @           0x412eb1 TestWaitingLogSink::send()
    @     0x7f892fb417d0 (unknown)
    @           0x412eb1 TestWaitingLogSink::send()
    @     0x7f89304f7f06 google::LogMessage::SendToLog()
    @     0x7f89304f35af google::LogMessage::Flush()
    @     0x7f89304f3739 google::LogMessage::~LogMessage()
    @           0x408cf4 TestLogSinkWaitTillSent()
    @           0x4115de main
    @     0x7f892f7ef1c4 (unknown)
    @           0x4046f9 (unknown)
```

#### Customizing Handler Output

By default, the signal handler writes the failure dump to the standard error. However, it is possible to customize the destination by installing a callback using the `google::InstallFailureWriter()` function. The function expects a pointer to a function with the following signature:

```cpp
void YourFailureWriter(const char* message, std::size_t length);
```

> **Possible overflow errors**
>
> Users should not expect the `message` string to be null-terminated.

#### User-defined Failure Function

`FATAL` severity level messages or unsatisfied `CHECK` condition terminate your program. You can change the behavior of the termination by `google::InstallFailureFunction`.

```cpp
void YourFailureFunction() {
  // Reports something...
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  google::InstallFailureFunction(&YourFailureFunction);
}
```

By default, glog tries to dump the stacktrace and calls `std::abort`. The stacktrace is generated only when running the application on a system supported[1](https://google.github.io/glog/stable/failures/#fn:1) by glog.

------

1. To extract the stack trace, glog currently supports the following targets:
   - x86, x86_64,
   - PowerPC architectures,
   - `libunwind`,
   - and the Debug Help Library (`dbghelp`) on Windows.

### Automatically Remove Old Logs

To enable the log cleaner:

```cpp
using namespace std::chrono_literals;
google::EnableLogCleaner(24h * 3); // keep your logs for 3 days
```

In C++20 (and later) this can be shortened to:

```cpp
using namespace std::chrono_literals;
google::EnableLogCleaner(3d); // keep your logs for 3 days
```

And then glog will check if there are overdue logs whenever a flush is performed. In this example, any log file from your project whose last modified time is greater than 3 days will be `unlink`()ed.

This feature can be disabled at any time (if it has been enabled) using

```cpp
google::DisableLogCleaner();
```

### Strip Logging Messages

Strings used in log messages can increase the size of your binary and present a privacy concern. You can therefore instruct glog to remove all strings which fall below a certain severity level by using the `GOOGLE_STRIP_LOG` macro:

If your application has code like this:

```cpp
#define GOOGLE_STRIP_LOG 1    // this must go before the #include!
#include <glog/logging.h>
```

The compiler will remove the log messages whose severities are less than the specified integer value. Since `VLOG` logs at the severity level `INFO` (numeric value `0`), setting `GOOGLE_STRIP_LOG` to 1 or greater removes all log messages associated with `VLOG`s as well as `INFO` log statements.

### System-specific Considerations

#### Notes for Windows Users

glog defines the severity level `ERROR`, which is also defined by `windows.h`. You can make glog not define `INFO`, `WARNING`, `ERROR`, and `FATAL` by defining `GLOG_NO_ABBREVIATED_SEVERITIES` before including `glog/logging.h`. Even with this macro, you can still use the iostream like logging facilities:

```cpp
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <windows.h>
#include <glog/logging.h>

// ...

LOG(ERROR) << "This should work";
LOG_IF(ERROR, x > y) << "This should be also OK";
```

However, you cannot use `INFO`, `WARNING`, `ERROR`, and `FATAL` anymore for functions defined in `glog/logging.h`.

```cpp
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <windows.h>
#include <glog/logging.h>

// ...

// This won’t work.
// google::FlushLogFiles(google::ERROR);

// Use this instead.
google::FlushLogFiles(google::GLOG_ERROR);
```

If you don't need `ERROR` defined by `windows.h`, there are a couple of more workarounds which sometimes don't work[1](https://google.github.io/glog/stable/windows/#fn:1):

- `#define WIN32_LEAN_AND_MEAN` or `NOGDI` **before** `#include <windows.h>`.
- `#undef ERROR` **after** `#include <windows.h>`.

#### Installation Notes for 64-bit Linux Systems

> **Note**
>
> The description on this page is possibly not up-to-date.

The [glibc built-in stack-unwinder](https://google.github.io/glog/stable/unwinder/#glibc-built-in-stack-unwinder) on 64-bit systems has some problems with glog. In particular, if you are using [`InstallFailureSignalHandler()`](https://google.github.io/glog/stable/failures/), the signal may be raised in the middle of `malloc`, holding some `malloc`-related locks when they invoke the stack unwinder. The built-in stack unwinder may call `malloc` recursively, which may require the thread to acquire a lock it already holds resulting in a deadlock.

##### Recommended Approach: `libunwind`

For above reason, if you use a 64-bit system and you need `InstallFailureSignalHandler()`, we strongly recommend you install `libunwind` before trying to configure or install google glog. libunwind can be found [here](http://download.savannah.nongnu.org/releases/libunwind/libunwind-snap-070410.tar.gz).

Even if you already have `libunwind` installed, you will probably still need to install from the snapshot to get the latest version.

> **Warning**
>
> If you install libunwind from the URL above, be aware that you may have trouble if you try to statically link your binary with glog: that is, if you link with `gcc -static -lgcc_eh ...`. This is because both `libunwind` and `libgcc` implement the same C++ exception handling APIs, but they implement them differently on some platforms. This is not likely to be a problem on ia64, but may be on x86-64.

Also, if you link binaries statically, make sure that you add `-Wl,--eh-frame-hdr` to your linker options. This is required so that `libunwind` can find the information generated by the compiler required for stack unwinding.

Using `-static` is rare, though, so unless you know this will affect you it probably won't.

##### Alternative Stack-unwinder

If you cannot or do not wish to install `libunwind`, you can still try to use two kinds of stack-unwinder:

###### glibc Built-in Stack-unwinder

As we already mentioned, glibc's unwinder has a deadlock issue. However, if you don't use `InstallFailureSignalHandler()` or you don't worry about the rare possibilities of deadlocks, you can use this stack-unwinder. If you specify no options and `libunwind` isn't detected on your system, the configure script chooses this unwinder by default.

###### Frame Pointer based Stack-unwinder

The frame pointer based stack unwinder requires that your application, the glog library, and system libraries like libc, all be compiled with a frame pointer. This is *not* the default for x86-64.
