Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/jammy64"

    # Files and Folders
    config.vm.synced_folder ".", "/data"

  # VM specific configs
  config.vm.provider "virtualbox" do |v|
    v.name = "SEcube-Samples"
    v.gui = false
    v.memory = 8192
    v.cpus = 4

    # USB
    v.customize ["modifyvm", :id, "--usb", "on"]
    # v.customize ["modifyvm", :id, "--usbehci", "on"]  # USB 2.0
    v.customize ["modifyvm", :id, "--usbxhci", "on"]    # USB 3.0
    v.customize ["usbfilter", "add", "0",
      "--target",       :id,
      "--name",         "SECube",
      "--manufacturer", "SECube",
      "--product",      "SECube"]
  end
end