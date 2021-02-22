#include <node_api.h>

#include <windows.h>
#include <string>
#include <psapi.h>

// int processPid, string processName -> int
napi_value IsProcessRunning(napi_env env, napi_callback_info info)
{
  napi_status status;
  size_t argc = 1;
  napi_value argv[2];
  napi_value napiResult;

  int processPid = 0;
  int result = 1;
  char processNameA[1000];
  size_t processNameARead = 0;
  char procNameBuffer[10000];


  status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to parse arguments");
  }

  status = napi_get_value_int32(env, argv[0], &processPid);

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Invalid processId was passed as argument");
  }

  status = napi_get_value_string_latin1(env, argv[1], processNameA, 1000, &processNameARead);

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Invalid processName was passed as argument");
  }

  if (processPid != 0)
  {
    auto handle = OpenProcess (
      PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processPid);

    if (handle != NULL)
    {
      DWORD cr;
      if ((GetExitCodeProcess (handle, &cr) == 0) || (cr != STILL_ACTIVE))
      {
        // Process with pid is not running
        result = 0;
      }
      else
      {
        if (!GetProcessImageFileName (handle, procNameBuffer, 10000))
        {
          CloseHandle (handle);
          throw std::exception ("Could not get webservice process name");
        }
        CloseHandle (handle);

        std::string processName(processNameA);
        std::string processPath(procNameBuffer);

        if (processPath.find (processName) == std::string::npos)
        {
          // Process with pid found, but different name
          result = 0;
        }
      }
    }
    else
    {
      // Process with pid is not running
      result = 0;
    }
  }
  
  status = napi_create_int32(env, result, &napiResult);

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to create return value");
  }

  return napiResult;
}


napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_value fn;

  status = napi_create_function(env, NULL, 0, IsProcessRunning, NULL, &fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to wrap IsProcessRunning native function");
  }

  status = napi_set_named_property(env, exports, "is_process_running", fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to populate is_process_running exports");
  }

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)