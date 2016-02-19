function Initialize() { }

function Process(request) {
  if (options.verbose) {
    log("Processing " + request.host + request.path +
        " from " + request.referrer + "@" + request.userAgent);
  }
  if (!output[request.host]) {
    output[request.host] = 1;
  } else {
    output[request.host]++
  }
}

Initialize();
