from distutils.version import LooseVersion
from . codes import CMDTYPE, LEDTYPE, SPIChipsets, BufferChipsets
from bibliopixel.drivers.return_codes import RETURN_CODES, print_error
from bibliopixel.util import log, util
from bibliopixel.project.importer import import_symbol


class Devices(object):
    """Manage a list of serial devices."""

    def __init__(self, hardware_id, baudrate):
        self.hardware_id = hardware_id
        self.baudrate = baudrate
        self.serial = import_symbol('serial')
        self.list_ports = import_symbol('serial.tools.list_ports')

    def find_serial_devices(self):
        self.devices = {}
        hardware_id = "(?i)" + self.hardware_id  # forces case insensitive

        for ports in self.list_ports.grep(hardware_id):
            port = ports[0]
            id = 0
            ver = 0
            if getattr(ports, '__len__', lambda: 0)():
                log.debug('Multi-port device %s:%s:%s with %s ports found',
                          self.hardware_id, id, ver, len(ports))
            if id < 0:
                log.debug('Serial device %s:%s:%s with id %s < 0',
                          self.hardware_id, id, ver)
            else:
                self.devices[id] = port, ver

        return self.devices

    def get_device(self, id=None):
        if id is None:
            if not self.devices:
                raise ValueError('No default device for %s' % self.hardware_id)
            id, (device, version) = sorted(self.devices.items())[0]

        elif id in self.devices:
            device, version = self.devices[id]

        else:
            error = 'Unable to find device with ID %s' % self.deviceID
            log.error(error)
            raise ValueError(error)

        log.info("Using COM Port: %s, Device ID: %s, Device Ver: %s",
                 device, id, version)
        return id, device, version

    def error(self):
        error = "There was an unknown error communicating with the device."
        log.error(error)
        raise IOError(error)
