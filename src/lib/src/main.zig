const std = @import("std");
const windows_api = @import("std").os.windows;
usingnamespace windows_api;

extern "user32" stdcallcc fn MessageBoxA(?HANDLE, ?LPCTSTR, ?LPCSTR, UINT) void;

pub fn WinMain(hInstance: HANDLE, hPrevInstance: HANDLE, lpCmdLine: LPWSTR, nCmdShow: c_int) c_int {
    MessageBoxA(null, c"Blah", c"Ok", 0x20);
}

// pub fn main() void {
//     // std.debug.warn("Test");
//     const title = c"Some title";
//     const text = c"Some text.";
//     MessageBoxA(null, title, text, 0x20);
// }
