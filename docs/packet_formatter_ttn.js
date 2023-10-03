function decodeUplink(input) {
  if (input.bytes.length == 25) {
    var packet_type = (input.bytes[0] === 1) ? 'satellite' : 'terrestrial';

    var send_epoch = (input.bytes[4] << 0) | (input.bytes[3] << 8) | (input.bytes[2] << 16) | (input.bytes[1] << 24);

    var gnss_latitude = (input.bytes[8] << 0) | (input.bytes[7] << 8) | (input.bytes[6] << 16) | (input.bytes[5] << 24);
    gnss_latitude /= 1e6;
    var gnss_longitude = (input.bytes[12] << 0) | (input.bytes[11] << 8) | (input.bytes[10] << 16) | (input.bytes[9] << 24);
    gnss_longitude /= 1e6;

    var next_pass_epoch = (input.bytes[16] << 0) | (input.bytes[15] << 8) | (input.bytes[14] << 16) | (input.bytes[13] << 24);

    var next_pass_duration = (input.bytes[20] << 0) | (input.bytes[19] << 8) | (input.bytes[18] << 16) | (input.bytes[17] << 24);

    var next_gnss_update_epoch = (input.bytes[24] << 0) | (input.bytes[23] << 8) | (input.bytes[22] << 16) | (input.bytes[21] << 24);

    return {
      data: {
        packet_type: packet_type,
        send_epoch: send_epoch,
        gnss_latitude: gnss_latitude,
        gnss_longitude: gnss_longitude,
        next_pass_epoch: next_pass_epoch,
        next_pass_duration: next_pass_duration,
        next_gnss_update_epoch: next_gnss_update_epoch,
        raw_data: input.bytes
      },
      warnings: [],
      errors: []
    };
  }
  else {
    return {
      data: {
        raw_data: input.bytes
      },
      warnings: ["Unrecognized payload"],
      errors: []
    };
  }
}