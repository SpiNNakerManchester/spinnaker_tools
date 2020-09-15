<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>sark.h</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/include/</path>
    <filename>sark_8h.html</filename>
    <includes id="spinnaker_8h" name="spinnaker.h" local="no" imported="no">spinnaker.h</includes>
    <includes id="version_8h" name="version.h" local="no" imported="no">version.h</includes>
    <class kind="struct">divmod</class>
    <class kind="struct">mem_link</class>
    <class kind="struct">mem_block</class>
    <class kind="struct">srom_data</class>
    <class kind="struct">rtr_entry</class>
    <class kind="struct">app_data</class>
    <class kind="struct">sdp_msg</class>
    <class kind="struct">sdp_hdr</class>
    <class kind="struct">cmd_hdr</class>
    <class kind="struct">block</class>
    <class kind="struct">heap_t</class>
    <class kind="struct">event_vec</class>
    <class kind="struct">sark_vec</class>
    <class kind="struct">vcpu</class>
    <class kind="struct">event</class>
    <class kind="struct">proc_queue</class>
    <class kind="struct">pkt</class>
    <class kind="struct">event_data</class>
    <class kind="struct">sark_data</class>
    <class kind="struct">sv</class>
    <member kind="define">
      <type>#define</type>
      <name>SARK_EVENT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad53b88aee1fc3aeea750b030563cb85e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NUM_SDP_MSGS</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad6eff7d653ca04b3398d701c8182fb99</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SARK_SLOT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aeb1d38814b019802b8e34aaa912e631d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INIT_EVENTS</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a653fe83d252a32d8d5ce65a1372841f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEAD_WORD</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a34d36536f69ec829a60d5fb243dd2cfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ntohs</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae886ad5b9af398395184d1bf093481c6</anchor>
      <arglist>(t)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>htons</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a9ae73de95a4f522c1dace01eab8ae063</anchor>
      <arglist>(t)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHIP_ID</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a20b085d7fd8f90ea0ed018cc74c08f27</anchor>
      <arglist>(x, y)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHIP_X</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a6dbfa8e49ec710299267b458040f8f16</anchor>
      <arglist>(id)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHIP_Y</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a027a20ed4ea1ef5a0c5b87777f1e8ea9</anchor>
      <arglist>(id)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>APLX_ENTRY_SIZE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2b562826d75a678a5f5656e3e262d776</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>APLX_BUF_SIZE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a10c56adc87e94ffa32287d49fe5aa6d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>APLX_BUFFER</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4c408316681bfd84d6ffe373585f5731</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TAG_NONE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a15fc8ad847508cf752f5debd1c038206</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TAG_HOST</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa68660d24c801e835ec97b7e541a6722</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_ALLOC_FIRST</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a62ca58ae605de746d1b10d17fe0b03c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_ALLOC_LAST</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a9104486bdb50a1bf8146a3f2e7755782</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_ALLOC_MAX</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a09d7af265bf828a5c6fad9ceb95e8e0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ALLOC_LOCK</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a170c14469b29096618dd05f78a9414c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ALLOC_ID</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a6ff0a91d1c257f90c16d72e4a48f619a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>A_LOCK_SEMA</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a849f3988621b83a38f7ddcfd5ee72b5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>A_STATE_RTE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a663ac8a882d1dc5f5835e334a8fb06f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EVENT_COUNT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a25425ecb642d0c2357851329b8ea1c88</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LED_ON</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aaa8988d6632fc64b0ec4a8528cebb7fd</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LED_OFF</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a552169688fe8834a280b877a8cf5020e</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LED_INV</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a40ac125daaf2b16529462ed1637244e2</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LED_FLIP</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aebf0428d662807a997b839e76d2d1e6f</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NUM_SDP_PORTS</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a321f6e755eca39f34a67bb72223fe7d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PORT_SHIFT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3b8e1ad5bc34269210d852edd533bc48</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PORT_MASK</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a753dc0d0821793f46a32babcf2087a82</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CPU_MASK</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa3cf6f0504db0209a04bb4a266289477</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PORT_ETH</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0393f315f127a8caa35b5d66c29853d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SDP_BUF_SIZE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0096e6ee6d2a16f2f252af0a8c023546</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VCPU_SIZE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a6060b669d3bbd8528c906953d482b12e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VCPU_RT_CODE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a792e094cb18a9678817268cab9ec8f6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VCPU_CPU_STATE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab7dcc8d21a6fda2015872afa0d87a40e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VCPU_APP_ID</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afba36169cb13c66bdfe0fbef16c40b7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SARK_MSG_INT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af848793a9b2d4e9ec085572513370ec9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SARK_SIG_INT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad8cb7bda8b7575fcf048185ba2bed463</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SARK_VIRT_CPU</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a92037fc3f1799ec52c6206f81fa8fd77</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SARK_RANDOM</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a477f51f167f11ee8d7dee3d3606f80da</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SARK_CPU_CLK</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac15e08aee77ea798812f3a094f71931a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>sw_error</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a70a68a061183156ed09c67e2e724b0c2</anchor>
      <arglist>(mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SVC_STACK</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4565ab3ae591c3fffcf293acff6f8f89</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IRQ_STACK</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae8c0e292976f8a91aec9e4a3cbaa00cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FIQ_STACK</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2098a7f33b4d50e1f11292ac4468a868</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IO_STD</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>abf839f1e83698e15b6eec7ff83b6e35e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IO_DBG</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae07ae92a99e4f6d39dfd0d1cabce34da</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IO_BUF</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a9ef82c62bd8ba080a719f3daeff6dcfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IO_NULL</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aae50cb72f8e2d59f8da0a102900ce015</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>int_handler</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add2659cdf5414e6ab00bc5c7471a07f8</anchor>
      <arglist>)(void)</arglist>
    </member>
    <member kind="typedef">
      <type>enum spin_lock_e</type>
      <name>spin_lock</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a21bcbf0f89b96b8f40e0953419f0d3c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum rte_code_e</type>
      <name>rte_code</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5d457fd1484275d32efeba70e75a9af0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum cpu_state_e</type>
      <name>cpu_state</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a6cda0afbb13c1566fcf78c2bbd746a98</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum event_type_e</type>
      <name>event_type</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0cede94eab5ba2dbda7021d9846fdcdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum event_state_e</type>
      <name>event_state</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a66ab81ec56acf7c1537b75df21154f8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum vic_slot_e</type>
      <name>vic_slot</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a64cbeb17d515d4a60718a93633648bd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum event_priority_e</type>
      <name>event_priority</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5554e4b7d32295045a3f7bbcbe1780ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum sw_err_mode_e</type>
      <name>sw_err_mode</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e9a700a97ff6b66bbc29d74a8ca5898</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum sync_bool_e</type>
      <name>sync_bool</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a324329d30936f98474493194a7e07847</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum shm_cmd_e</type>
      <name>shm_cmd</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab2da1959fb0fd7f5520919e558fbadfc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum signal_e</type>
      <name>signal</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a90e8f0529265435d3864e2d7533b7f8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>event_proc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a14fe7f6e37874a6a9bd576fc6afd6b3a</anchor>
      <arglist>)(uint, uint)</arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>sark_aplx_command</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a707dc476cdca6c9a8cd1695f25fe555a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>APLX_ACOPY</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a707dc476cdca6c9a8cd1695f25fe555aa87f9b6a1505b09b1d62a427cb02e68a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>APLX_RCOPY</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a707dc476cdca6c9a8cd1695f25fe555aa14a3db1b3e820917f084acee5b0cbcbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>APLX_FILL</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a707dc476cdca6c9a8cd1695f25fe555aadde259e49b3d9ddc8173d3627a1b5554</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>APLX_EXEC</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a707dc476cdca6c9a8cd1695f25fe555aaf36a74033d29f7566f01738b80845e69</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>APLX_END</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a707dc476cdca6c9a8cd1695f25fe555aa15d4e155cc151a4f70298600b052ed48</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>spin_lock_e</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5cee57b7a2c3c335a5155b8fad8958d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOCK_MSG</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5cee57b7a2c3c335a5155b8fad8958d4a2bd89e77b8ad338358834341c7dce43d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOCK_MBOX</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5cee57b7a2c3c335a5155b8fad8958d4ac7cd8fa7e071fac1859700e3388bd964</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOCK_ETHER</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5cee57b7a2c3c335a5155b8fad8958d4afaa69bad332ec2fd0216ea262ee668ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOCK_GPIO</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5cee57b7a2c3c335a5155b8fad8958d4ad4155cd6adef65f957edaca879bc0c6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOCK_API_ROOT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5cee57b7a2c3c335a5155b8fad8958d4a26842e7a97cc425a110d27d69a65e651</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOCK_SEMA</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5cee57b7a2c3c335a5155b8fad8958d4a251c7766d9bb74e4ce9c8aa5dd4e9b7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOCK_HEAP</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5cee57b7a2c3c335a5155b8fad8958d4a89d69bef15e6dd767c4d851969568b49</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOCK_RTR</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5cee57b7a2c3c335a5155b8fad8958d4a2403509583bec93127bb5a807ee64a90</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>app_flags_e</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ada83d5de50faddcc7a1a3a3354b3ec49</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>APP_FLAG_WAIT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ada83d5de50faddcc7a1a3a3354b3ec49a4e78619422e7eaaea3803e2bf1e5b55e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>rte_code_e</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_NONE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806af19712c673bff49b738641905254d963</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_RESET</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806a1dca7761a7d789527304b6bf9a9952aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_UNDEF</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806abfe89d504e69c2b906412d62d0ea5ea9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_SVC</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806a5f6cbc846580ec613b6856d4c763edfd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_PABT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806a74283c5c33207d02e53ef2315c84a20d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_DABT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806af3e999535726c0d7294855bf261abcc7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_IRQ</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806ab0e4d003206cf229551f8dd414434114</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_FIQ</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806af32ac3e03175989ee4c9ba3a55f5ac52</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_VIC</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806a9e8efcaeece564e534416a5f54ad5d5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_ABORT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806a228d90e8251d61de8cf4fe5026008998</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_MALLOC</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806acd9e88eb8d10980ccdd8c1b2bb3ec035</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_DIV0</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806a7b6e67cfddc6236ae6c74f2e22f220e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_EVENT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806ac566062eb6d8f30fb7ffbb6a7963044d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_SWERR</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806a66bc0d0be1de9f6afc1787e49ffae339</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_IOBUF</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806a80dda6ab0b06dcd07e8870765c9d2615</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_ENABLE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806a40f3f82c8cdd80ccbd9457be160d5b98</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_NULL</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806aadaf315a9f0d71ecb74bc0d2aa420996</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_PKT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806a00a3039990f8f9e2d2236286d0d9e786</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_TIMER</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806a1d402645d5abfe4e00a6130ff0c66dfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_API</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806ad86ab887d63c00784b2efa65fa1cbb87</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTE_VER</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3cf9347d757f8d978aa2d35927e21806ad19840e59921fc9dd40af8f932a282a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>sark_alib_rte_codes</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aba531b9d7bd6ab156cf29a04a66216fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>A_RTE_RESET</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aba531b9d7bd6ab156cf29a04a66216fba3d06b52632231b657557e80993907f6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>A_RTE_UNDEF</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aba531b9d7bd6ab156cf29a04a66216fbaff8fe5f17e0c79da8b730074ec4e1869</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>A_RTE_SVC</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aba531b9d7bd6ab156cf29a04a66216fbad0c8329749e6d90e83f091c131138b2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>A_RTE_PABT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aba531b9d7bd6ab156cf29a04a66216fbad79d30396f3ed5e46770ed59d70ddbed</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>A_RTE_DABT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aba531b9d7bd6ab156cf29a04a66216fba58976896638075c1f09e985fc288bc2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>A_RTE_IRQ</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aba531b9d7bd6ab156cf29a04a66216fba85ba342ddd7755f309884b75ac7b79ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>A_RTE_FIQ</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aba531b9d7bd6ab156cf29a04a66216fba4e2805a1e9e04a279c566eb9060d46cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>A_RTE_VIC</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aba531b9d7bd6ab156cf29a04a66216fbacb62844ebe76904b9da8331ee1de8e47</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>cpu_state_e</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_DEAD</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236da5fdc95ad5a4ddcf32e79798c8b2c53b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_PWRDN</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236dab6112bf8f882083d1481cdc949b86b54</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_RTE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236daea3adbea49fd3cf9d2bffa4698ebe03d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_WDOG</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236da081b5c672eb66e504283de013ffaeeb9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_INIT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236da7bebf57001a32d7dd3203e3ef346f37a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_WAIT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236dad92a650aabe37b8b491eaff36fe15318</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_SARK</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236dab06591102cb34c05e61dc151bb812d59</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_RUN</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236da504ca9cb33bebc4ff8b20b3eb97facbd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_SYNC0</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236da6fa07205ec7c136824a44685895becf7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_SYNC1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236da55fddfc9f3fe2b6429523deb00c458b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_PAUSE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236dadcefe4af17b67dc5de698473127ea564</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_EXIT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236dae259fb0c2645704951236737a63322ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_12</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236da00c5193a3d21fe121673fd5e7b23f0f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_13</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236da40e94686da942cc278f596f06a68ba39</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_14</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236da23871c9467df994d98fd777deec61d99</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_STATE_IDLE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37318835424d76f3f5b321058887236da20cbc4e49e69ce2f28bd2589defaf502</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>event_type_e</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab9a826a649bbc6b8084e7c7d6bc23881</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVENT_TIMER</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab9a826a649bbc6b8084e7c7d6bc23881a811c2a26881d48cc1a77300cd79a918e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVENT_RXPKT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab9a826a649bbc6b8084e7c7d6bc23881aec3d6aec2dfd7c4de1c10ca9a2372795</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVENT_SDP</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab9a826a649bbc6b8084e7c7d6bc23881abf41b11fa8ff57c33f446088c38712bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVENT_USER</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab9a826a649bbc6b8084e7c7d6bc23881a822ec7de5e51e61b1e86620be23fe0bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVENT_SIG</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab9a826a649bbc6b8084e7c7d6bc23881a0faa65d8fd5b5a4659b6473af6f941fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVENT_DMA</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab9a826a649bbc6b8084e7c7d6bc23881a21b293748ec91ca885a07f6c9b218ea5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVENT_MAX</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab9a826a649bbc6b8084e7c7d6bc23881af66ca241c2a2e3ef806d69cb9fd7c339</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>event_state_e</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>acdb52540a2684e30650a8a4ac892f74a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVENT_RUN</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>acdb52540a2684e30650a8a4ac892f74aa589a97f39a2de0129bf372b1fd4216fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVENT_PAUSE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>acdb52540a2684e30650a8a4ac892f74aa4a95b1a327b7f948875054edc5c2f76b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EVENT_STOP</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>acdb52540a2684e30650a8a4ac892f74aac9bede468864b3ea2574a7b355cf6590</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>vic_slot_e</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a680660a112faeb990fc97a25eef9d643</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SLOT_FIQ</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a680660a112faeb990fc97a25eef9d643a2cf274b6e5590191aacba5d191d5162c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SLOT_MAX</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a680660a112faeb990fc97a25eef9d643aa1cadb71ce46d1447d36d3c7ec5cb53c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>event_priority_e</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a1a22a8c1bf3352bc25f568010a38279b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>sw_err_mode_e</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a29002882868350d94a53ef964eddcdbe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SW_OPT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a29002882868350d94a53ef964eddcdbea708f4708e456e4554f57a99e34e5d22f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SW_NEVER</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a29002882868350d94a53ef964eddcdbea370be05a6e6dc866854719387984ab6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SW_RTE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a29002882868350d94a53ef964eddcdbeaeb6866cff0b528dbc9f449804f20cdc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>sync_bool_e</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7f5f3be8c06b875eff1b6e7501ca8446</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SYNC_NOWAIT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7f5f3be8c06b875eff1b6e7501ca8446a22d97c7711134391b3bc55ddf432e25b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SYNC_WAIT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7f5f3be8c06b875eff1b6e7501ca8446a18a6fa8889d7501469a972a54da8943c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>shm_cmd_e</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab6c27ffba6e45118ef6605f899911cf2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SHM_IDLE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab6c27ffba6e45118ef6605f899911cf2a365e33483054f2d08201b35faf6ced36</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SHM_MSG</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab6c27ffba6e45118ef6605f899911cf2ac6e915c4e758a768bba1d80c69ec1d75</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SHM_NOP</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab6c27ffba6e45118ef6605f899911cf2aecb3639fc13f200ea2bd988a07cd754a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SHM_SIGNAL</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab6c27ffba6e45118ef6605f899911cf2a1dbdb8b836e3800024971f46dc61205b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SHM_CMD</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab6c27ffba6e45118ef6605f899911cf2aedc5209f9eafcb96717369f76a07f81f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>signal_e</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_INIT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015ae06d9cbab66716ec34598717c5d973a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_PWRDN</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015a59d8d559f6300c81ed69a371bd131766</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_STOP</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015a5fb8c6a63805565f9e699c631ac15dfe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_START</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015aa4538103ce6a6d0e3c5f51b0b9333d51</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_SYNC0</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015a4ea0ddcd6167372159567acc48950b9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_SYNC1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015a2a90faf7dd00f16614bc2c3bd1d957db</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_PAUSE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015a4033030dfc14b878cc7767ad7a53a7f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_CONT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015a29026413e1d3f85a38f640074e5a0db7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_EXIT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015ae4eb5890f0cf58b082dd48469a74bd6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_TIMER</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015af24bd1d3addc94ce1c7ec81f8da905c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_USR0</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015a53cc3858df9643ebc82c0b04cc0538ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_USR1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015a71f5cfd2f4c4b67e88e559b6eb1004d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_USR2</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015a49a650cf8f35bf794982ca44e1af030e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_USR3</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e4e9fc0acec47f32a5cc3737c894015a6730124eaaa60ae2ff8858b1036ccf9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>sark_scp_command_codes</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_VER</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05a576fb2eb53263319ee51fd4152ff8fcc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_RUN</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05a51c06d6ddc76ff994a5aa1d25b71e57f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_READ</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05a43d7f3a35093d4198124c87b43afdefb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_WRITE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05a612986a7ae236cab656844222f00ba82</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_APLX</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05a3de37de83aeb28145a73805b4f4f5422</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_FILL</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05ac04c56b963737bed98a9765d300e60aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_REMAP</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05a1f6052e4d5cda4eeba5759bec798cd3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_LINK_READ</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05ac7dc40ac7d7b36b33599a9647741ec3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_LINK_WRITE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05ae1b0881e5b6d1168e83be5ca76b40249</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_AR</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05ac200c7f643146c6174a04d70dd7c6765</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_NNP</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05a9768ff77bcde46195196e6466bc0ab5e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_SIG</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05a027c8932fc4f1f58d96be1c9d3573348</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_FFD</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05ad11c5b33509ea0f84a1b50666bc0a4ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_AS</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05aab54e60c11c527ae91bb806c0806a7f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_LED</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05a41754a0808533384d7667e0c7c70bb28</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_IPTAG</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05ae15692462fd93686a1e4c99d2eaea246</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_SROM</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05a2cbdb534568d4ce50ae876dd98165dfb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_ALLOC</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05aa1aeafde66020145ebd8c2cb24b522b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_RTR</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05adee488b16055c349f995c986587bbe26</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_INFO</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05a6b1ac29fd084fc49f7c5c5e80130614b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CMD_TUBE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add51abc04ff5e1d219053f53ce83fa05afa121c234cf98fbef4bcf6da8a690c78</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>sark_scp_return_codes</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_OK</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0ab834a5bb2ce6d97c2831fe3d71103c14</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_LEN</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0ad6508f5b7837bba3ce27c3543b33a704</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_SUM</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0a83a9fda8ab6c52397676f85531a86348</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_CMD</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0a0f674dd3b0ad65f7afedd83a5b9c7416</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_ARG</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0a71a503467fc752c1a4e5e85917c840df</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_PORT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0ace382a22765636da1bf2fc60fc541da5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_TIMEOUT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0a6aa8355501388bd7a58471d19e70cfb6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_ROUTE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0a7d7f9289cf848b83eec0d37b7d0fbeb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_CPU</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0ab135f1136713a6f55b3dd46f44b992bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_DEAD</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0a7f57ddeaf846172d6ec6d1399e5417e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_BUF</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0a8ca11e666cfc90593029e15c0b2e1748</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_P2P_NOREPLY</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0a0ddc10648826626c3262a569627d0596</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_P2P_REJECT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0ace376f8bd2c08995e08dccabbe83c883</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_P2P_BUSY</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0ab97e74175c024de2876bcd9ac41d4c0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_P2P_TIMEOUT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0aa7dc80dad0b678969118ec0410c475d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RC_PKT_TX</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5dd23bc996e5f56e5f45964dc4e3fff0a93c187db9e33ca37865ba1e5cfbaff3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>sark_scp_memory_size_types</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a61d3deefebff466d46ecda52171bd47d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TYPE_BYTE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a61d3deefebff466d46ecda52171bd47da66aba953e1da3345bc207065c05c28d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TYPE_HALF</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a61d3deefebff466d46ecda52171bd47da793a9db18e0c2fd7a6b519c6453026d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TYPE_WORD</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a61d3deefebff466d46ecda52171bd47dadcd42ea06b0d39bc3f09e2a6abf0b5f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cpu_irq_enable</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a082cf1e7488833e20c56430f4dc6667c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cpu_irq_disable</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab088513d34540c8ad53a67deb34e6f7a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cpu_fiq_enable</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad65dc08d0addbd5e56abc418252de701</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cpu_fiq_disable</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7d3c1edeca108523719403b2f9b0772e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cpu_int_enable</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a066ce48b9e8823e9535c533ce68a5147</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cpu_int_disable</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a14ef80db94e3bc22ed0c13951c5b777f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cpu_int_restore</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a475a13902c8055eb6aea178f8be3c6bb</anchor>
      <arglist>(uint cpsr)</arglist>
    </member>
    <member kind="function">
      <type>uint *</type>
      <name>cpu_get_sp</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a22e95f32619aef5e01d518e529b70ce9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cpu_get_cpsr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2dd4568f06db076d687fa3f4435becc4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cpu_set_cpsr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a856899694c28cdf55a40931ef6bbdb12</anchor>
      <arglist>(uint cpsr)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cpu_get_cp15_cr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a74c64301dd0a88a9c2c6fd2c28234be2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cpu_set_cp15_cr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aed1c1643cd51f2e97e26d582e53cc159</anchor>
      <arglist>(uint value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cpu_wfi</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a96973cac0a4eb45f83b30785b8fc77ca</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cpu_sleep</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0756562c562df4adb431af181f867711</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cpu_shutdown</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4e5a88149045cde1e775328022b35d9d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rt_error</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>abe2018e60bb35fb629cdbb057368a2f4</anchor>
      <arglist>(uint code,...)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_mem_cpy</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a6cbb47391ebe5563f3d43a343cb03f51</anchor>
      <arglist>(void *dest, const void *src, uint n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_str_cpy</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad58e22edc7b04c1a8ed25d7ada9f46f5</anchor>
      <arglist>(char *dest, const char *src)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_str_len</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aeef96491568c6a6468cb4d35d1d18d07</anchor>
      <arglist>(char *string)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_cpu_state</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8c3cb686bab55b58b206f7f13b7ecafa</anchor>
      <arglist>(cpu_state state)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_msg_cpy</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac420f512079b7df88c0da0aae266cd5b</anchor>
      <arglist>(sdp_msg_t *to, sdp_msg_t *from)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_word_cpy</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a9510d06461b092e5e2d4538147ed010e</anchor>
      <arglist>(void *dest, const void *src, uint n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_word_set</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aea3f9c1bfb731a51ab1f969e3a45a8b9</anchor>
      <arglist>(void *dest, uint data, uint n)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_lock_get</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7e7203fdd0bd3f58c51b7f63eb0ebfb9</anchor>
      <arglist>(spin_lock lock)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_lock_free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a10119efa59b363c6a6a9ac6703220096</anchor>
      <arglist>(uint cpsr, spin_lock lock)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_sema_raise</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add7535e9fe6473c492496134dcb378a7</anchor>
      <arglist>(uchar *sema)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_sema_lower</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a34fbeff6ae25da166f9895891da4d585</anchor>
      <arglist>(uchar *sema)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_app_raise</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad8d3315f83d10fc3071fc74ddc239248</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_app_lower</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad0488210f65d863dd3a609c71ae34a1b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_app_sema</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae711485b0ed4f29a98a08f4476e6c1ed</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_app_cores</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7b765a16adc6f4a88ac9017b9202ba83</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_app_lead</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>affdba350f69ec3d46d9d559571feb0f6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_srand</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a1e4b606aff6663055d67d9bae754180d</anchor>
      <arglist>(uint seed)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_rand</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3d2f834ac2db53f8e42a422916ced870</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>divmod_t</type>
      <name>sark_div10</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a527ad9bc03c755cf19b0bd1177d704d3</anchor>
      <arglist>(uint n)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_count_bits</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2899892c855c4e5260c0b59086ad3866</anchor>
      <arglist>(uint word)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_bx</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a33e93143297356f647563cd10400a2a0</anchor>
      <arglist>(uint addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_aplx</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afeae6d5b8f962fb2c1dfb1ac1c39f0c1</anchor>
      <arglist>(uint *table, uint app_id)</arglist>
    </member>
    <member kind="function">
      <type>uint *</type>
      <name>cpu_init_mode</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a01d9f9030e4539662cad6650b6694fb7</anchor>
      <arglist>(uint *stack, uint mode, uint size)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>chksum_64</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a13db093995345a88674c13e7906d4ca3</anchor>
      <arglist>(uint a, uint b)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>chksum_32</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae6249db4a87d742ba896db808317dea2</anchor>
      <arglist>(uint a)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_chip_id</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa10c5c53ee9f467e3aad6e1a054cc466</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_core_id</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a57fab81f030fe93ea4f247f2d27f1e53</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_app_id</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5a95df8505e42ee47d56f3bb752a8ddc</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static char *</type>
      <name>sark_app_name</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4bef6fbe3bbf6c0b489a2435768d6703</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>sdp_msg_t *</type>
      <name>sark_msg_get</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab3ab790eb60f2c24aa9b20fff884e64b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_msg_free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8b6bbbfa956f793ea41c8702879cffbd</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_msg_send</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4e20f66a6e7058940cdbfdd077fbcdc4</anchor>
      <arglist>(sdp_msg_t *msg, uint timeout)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_delay_us</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a163a54c093dfe7ad72239509b663fdcb</anchor>
      <arglist>(uint delay)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sw_error_fl</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa82adc6677b15f452e59063cf4a8f2f0</anchor>
      <arglist>(sw_err_mode mode, char *file, uint line)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_cmd_ver</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a10a6d37b96c4cd0339de66851e9852e1</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_cmd_read</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a05105db095cf54052c1e00cb2d03424b</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_cmd_write</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a214cec2f71f5dc818d88c0abc398fe29</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_cmd_fill</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af67d477747aa6b62a2448590a7f4ed2d</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>sark_block_init</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a05a451a1b870252451ae5661990db041</anchor>
      <arglist>(void *buf, uint size, uint count)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>sark_block_get</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a93d1de5c5e2f2a1607c05eb6f024a183</anchor>
      <arglist>(mem_block_t *root)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_block_free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3d40f5611a6a76a083d0751d2ffcebc3</anchor>
      <arglist>(mem_block_t *root, void *blk)</arglist>
    </member>
    <member kind="function">
      <type>sdp_msg_t *</type>
      <name>sark_shmsg_get</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a1bdfaf0e7bfe553bcafc6d04bf321bf1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_shmsg_free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a69f3c808f1b7045354a386c2bd2872df</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_call_cpp_constructors</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2885d11fa603c565e9084ffffc3fe557</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>io_printf</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a24e7b80208352d98dec4cbed92522afe</anchor>
      <arglist>(char *stream, char *format,...)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>io_put_char</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae3d6e0b4f6796acf8fb2dc494453598f</anchor>
      <arglist>(char *stream, uint c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_io_buf_reset</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afe00ab22d6ec00b7fc698270e7be8c28</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>sark_alloc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a30d1e6e1b258fa76d6412827a1aa4f15</anchor>
      <arglist>(uint count, uint size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a40ce895f48f22b29d2aefc923b38b280</anchor>
      <arglist>(void *ptr)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>sark_xalloc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8d1f83786e86e8943b1e1a2d8fa68b85</anchor>
      <arglist>(heap_t *heap, uint size, uint tag, uint flag)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_xfree</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae4289de17bb19977ccbc8b1f680b4a6a</anchor>
      <arglist>(heap_t *heap, void *ptr, uint flag)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_xfree_id</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5197d7e0234aae820a2ddc722aeecbd4</anchor>
      <arglist>(heap_t *heap, uint app_id, uint flag)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_heap_max</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8e89c52519c74f83f002becac03d9ff3</anchor>
      <arglist>(heap_t *heap, uint flag)</arglist>
    </member>
    <member kind="function">
      <type>heap_t *</type>
      <name>sark_heap_init</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab76688949797930a80f58584467243c2</anchor>
      <arglist>(uint *base, uint *top)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_alloc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>abb9d9cc3e46a707969eb592b258744e5</anchor>
      <arglist>(uint size)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_alloc_id</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2d7bb03f41c2c68ad7fdb944bb23af64</anchor>
      <arglist>(uint size, uint app_id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5cb6e9bbf8425f6fe224833f6239c4ba</anchor>
      <arglist>(uint entry, uint clear)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_free_id</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7e038cf666dba5f8be35bb8cb8eead45</anchor>
      <arglist>(uint app_id, uint clear)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_alloc_max</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af89de816e4c181b8f24ba21873b24a27</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>sark_tag_ptr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a226f6ac4ac38c44dce34a617ed1f1aa6</anchor>
      <arglist>(uint tag, uint app_id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_vic_init</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aed572d885f25c88acdc5a11c26e51b2c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_vic_set</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8e5523f51f1df77463689f9567c87884</anchor>
      <arglist>(vic_slot slot, uint interrupt, uint enable, int_handler handler)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_led_init</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7338a87d005d821ee1ef167eda592ecb</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_led_set</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a74a3a76206602b65fc8f482d0c9cce5d</anchor>
      <arglist>(uint leds)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_mc_clear</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afc00e275bcd21290b0ade7caffb3f690</anchor>
      <arglist>(uint start, uint count)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_mc_init</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a151645129faec74d427f2961d4ab9449</anchor>
      <arglist>(uint start)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_mc_load</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3ba7cd3d8b44e6225eec84c46d20a441</anchor>
      <arglist>(rtr_entry_t *e, uint count, uint offset, uint app_id)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_mc_set</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a81ca61e30076d1724a68027b63f6e970</anchor>
      <arglist>(uint entry, uint key, uint mask, uint route)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_mc_get</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>acfe404b27b699071843bb9fbe10a5325</anchor>
      <arglist>(uint entry, rtr_entry_t *r)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_fr_set</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a71b1b5e0a5526750b671ebe436da92f3</anchor>
      <arglist>(uint route)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_fr_get</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab87c4fc234cb78333321bc0acf3b7b75</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_p2p_init</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a74ab37d7c02d8246d2b0fdc7670ce5fa</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_p2p_set</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a42ab52595bb1286fe7428cad3f2670d9</anchor>
      <arglist>(uint entry, uint value)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_p2p_get</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae43c60a896bbc207a45724881f051e6a</anchor>
      <arglist>(uint entry)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_diag_init</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4a25c25939b98a0966b1248a52f06f27</anchor>
      <arglist>(const uint *table)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_init</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2550bb97884d308a9cccd5f4791e6de8</anchor>
      <arglist>(uint monitor)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>v2p_mask</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a28c92abd55df0daf0b4b05284277c8b8</anchor>
      <arglist>(uint virt_mask)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pl340_init</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5bfda3e690ab2344e88f6e8934e2edd4</anchor>
      <arglist>(uint mem_clk)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>event_alloc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a06ace566279f8f98e6bf001dee74eeab</anchor>
      <arglist>(uint events)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a05a1933cd3173bcfe01e10150532b933</anchor>
      <arglist>(event_t *e)</arglist>
    </member>
    <member kind="function">
      <type>event_t *</type>
      <name>event_new</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac3dc841f217092d14cd043929c4ef5a0</anchor>
      <arglist>(event_proc proc, uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_config</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a28d6272010636e23c96102c17baa13b6</anchor>
      <arglist>(event_t *event, event_proc proc, uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pkt_tx_k</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5950f6444f3c7afd3e6cdefec311a19d</anchor>
      <arglist>(uint key)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pkt_tx_kd</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5c544176d5d81edfa1485b5ae2e0d20f</anchor>
      <arglist>(uint key, uint data)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pkt_tx_kc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0bace2238c993312c518553cdb237743</anchor>
      <arglist>(uint key, uint ctrl)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pkt_tx_kdc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa5737fbb29e2f0f47e0375e4cba4162d</anchor>
      <arglist>(uint key, uint data, uint ctrl)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>event_start</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afa72805def2820951b0e8f1967d518fd</anchor>
      <arglist>(uint period, uint events, sync_bool wait)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_pause</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a65e02c560e39a7afafbb5da4255906f4</anchor>
      <arglist>(uint pause)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_stop</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a05b364e4b7263b9e979f4f5d02369fa3</anchor>
      <arglist>(uint rc)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_wait</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8d885d4db63c2de6f5982e74e86cb84f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>event_queue</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a607f8bbe9eb8a7d99a4a335be0673b09</anchor>
      <arglist>(event_t *e, event_priority priority)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>event_queue_proc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a99da351948b0933e1a8f46db6d9b5f7d</anchor>
      <arglist>(event_proc proc, uint arg1, uint arg2, event_priority priority)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_run</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a6c997d47394c965abda9a03d8db70ae6</anchor>
      <arglist>(uint restart)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_register_int</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a59a64a46470fbaa6bf1a129eae284a25</anchor>
      <arglist>(event_proc proc, event_type event, vic_slot slot)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_register_queue</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>abcdc191f66af37a6b0972dca4b80159f</anchor>
      <arglist>(event_proc proc, event_type event, vic_slot slot, event_priority priority)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_register_pause</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a6c35de6a8f9072dc49db8984bd7ce135</anchor>
      <arglist>(event_proc proc, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_enable</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afc420e5045a73751d480bf2edfb15e72</anchor>
      <arglist>(event_type event_num, uint enable)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_register_timer</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a24995141a580df346fce85a38cbec1b8</anchor>
      <arglist>(vic_slot slot)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_register_pkt</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4175f7e6597dd3d01fff5b4afa13ea58</anchor>
      <arglist>(uint queue_size, vic_slot slot)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>event_user</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0354578d007f0031b66bb376939aee54</anchor>
      <arglist>(uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer_schedule</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a472cfc21a621f8e5153f1191342c5bc1</anchor>
      <arglist>(event_t *e, uint time)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>timer_schedule_proc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aab4e6e587060d426aa55638664f1e3b6</anchor>
      <arglist>(event_proc proc, uint arg1, uint arg2, uint time)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer_cancel</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aacd1585a3489d9ee70bf5805214fe7f2</anchor>
      <arglist>(event_t *e, uint ID)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer_cancel_init</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a494d1ae311642ab308b728ecfe310342</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static sark_vec_t *const</type>
      <name>sark_vec</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a1b3637d14687e77df4a7146100b306b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>sark_data_t</type>
      <name>sark</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a89aedee21d244fb4bf7613609168c7b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>event_data_t</type>
      <name>event</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae7a7108766201ba6bd821b143da63f09</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>build_date</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4f996c4cec2bbd8ef1c1bc86abdd2c35</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>build_name</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a691b98e25983f8eb56e33e563b250b6c</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SVC_STACK</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4565ab3ae591c3fffcf293acff6f8f89</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IRQ_STACK</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae8c0e292976f8a91aec9e4a3cbaa00cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FIQ_STACK</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2098a7f33b4d50e1f11292ac4468a868</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IO_STD</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>abf839f1e83698e15b6eec7ff83b6e35e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IO_DBG</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae07ae92a99e4f6d39dfd0d1cabce34da</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IO_BUF</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a9ef82c62bd8ba080a719f3daeff6dcfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IO_NULL</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aae50cb72f8e2d59f8da0a102900ce015</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_SSIZE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>adfd6c958be86add71bdb7f3e52a4f385</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_USIZE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa947c493823d4dfffbfcde71ef599ee7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_ISIZE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3a9f7f4562d47d4e3976c53557d28cbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_VSIZE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af81947df8a77e0baa70a86fe19757d40</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_RSIZE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa88d62a610a50e46910769547b25fa37</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_SIZE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4940f17eb2b3513af4d939c84ece4701</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_SROM</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad628bb4c6156bcfeb5b4237811808672</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_UBASE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af0519e7df1550cf253b850bc32830527</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_IBASE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>acea922b58cbe7b48956fb9de1a903fa1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_SV</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae64acf62910ab7afdbbe57fbd92e3026</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYS_BOOT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae3aec779e29c69df8ce62a2630709316</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYS_USER_TOP</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad9fc081289f30cbfe7ced12a71d13aea</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYS_USER_BASE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ada0ffbe52e900e6efd670af0e0351979</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_VECTORS</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a993ec852f04f051ca86338808f053b93</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_RANDOM</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4ea54a1b50b0f5d74df91b604006bca9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_VCPU</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a06e96a522d4adf97fbb2549806f73cfb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SV_SPARE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a41cd20bd99299a9ba4fbcc09768fb83e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SPARE_SIZE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0140dd12f36d8942f005aab7c7f73f38</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SROM_FLAG_BASE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a12328d30c230f38906d77eb722495965</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STATUS_MAP_BASE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad12358086249cb7c260cc4e88159f04d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RST_BLOCK_BASE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>abe8b3fcdb179c600393eb4eeef133d0d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SRF_PRESENT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0d35944bd3dd07fdd09a06b6bf4207ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SRF_HW_VER</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8713198899fda6df6178804b9f620f47</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SRF_PHY_INIT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a579398e90b577981f0e6a7f36947332a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SRF_PHY_RST</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af1343248569d9c13924ab90a2e253c51</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SRF_PHY_WAIT</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3116430639c64d108e5a37172317be9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SRF_ETH</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4817bfd950164146c30dc577eb339bbe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SRF_NONE</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a9e6962f26c59c9720644ba44488b8c1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static sv_t *const</type>
      <name>sv</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a74ab0d3b2aa00fa87c323a46349fe3b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static vcpu_t *const</type>
      <name>sv_vcpu</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac08702c96eb539a7fbcebbc114424703</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>sv_srom</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7e9ac0115eb99b77dc347bd62b6bf30a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>sv_random</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>acbaac45f0245c38d11322539dc35e58a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>sv_vectors</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aca065bfe68bde3e3f92fb30ffc9a9db4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>sv_board_info</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a88b8ecddf793203645bc5cf0d671e40d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sark_cpp.h</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/include/</path>
    <filename>sark__cpp_8h.html</filename>
    <includes id="sark_8h" name="sark.h" local="no" imported="no">sark.h</includes>
  </compound>
  <compound kind="file">
    <name>spin1_api.h</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/include/</path>
    <filename>spin1__api_8h.html</filename>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <class kind="struct">diagnostics_t</class>
    <member kind="define">
      <type>#define</type>
      <name>SPINN_SYSRAM_BASE</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>aadf2680da1c5fff7dad9cd3ddf0dc824</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SPINN_SYSRAM_SIZE</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>ab24e54fd159f1c09b55f394557361093</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SPINN_SDRAM_BASE</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>ab8ecfa2aa9440abb63206cbab1320d42</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SPINN_SDRAM_SIZE</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a7497cebca4c9d9d04fdc112edefbefc5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TRUE</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>aa8cecfc5c5c054d2875c03e77b7be15d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FALSE</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>aa93f0eb578d23995850d61f7d61c55c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SUCCESS</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>aa90cac659d18e8ef6294c7ae337f6b58</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FAILURE</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a6d58f9ac447476b4e084d7ca383f5183</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NULL</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a070d2ce7b6bb7e5c05602aa8c308d0c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>callback_t</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>aa0453a7dfd80bc7f68b399e7aabd1053</anchor>
      <arglist>)(uint, uint)</arglist>
    </member>
    <member kind="enumvalue">
      <name>NUM_EVENTS</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a06fc87d81c62e9abb8790b6e5713c55ba375c9117363fc3a2454e271606f3e111</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MC_PACKET_RECEIVED</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a06fc87d81c62e9abb8790b6e5713c55ba3f5a7856598a4a28315eefe4c335f1dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DMA_TRANSFER_DONE</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a06fc87d81c62e9abb8790b6e5713c55ba6514b0da0e0910cb27c19ffcbedadf6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TIMER_TICK</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a06fc87d81c62e9abb8790b6e5713c55bad1110959cd9312a756c5fb1d0f0eab8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SDP_PACKET_RX</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a06fc87d81c62e9abb8790b6e5713c55ba64eb210a799be292ddd091d3079128d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>USER_EVENT</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a06fc87d81c62e9abb8790b6e5713c55ba888ff01aa0269261336d13fcf00ba9f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MCPL_PACKET_RECEIVED</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a06fc87d81c62e9abb8790b6e5713c55ba2bf8e4910be4f6577f10dbcdef13a270</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FR_PACKET_RECEIVED</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a06fc87d81c62e9abb8790b6e5713c55badcd6ee548cace65bbcfcc191579540b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FRPL_PACKET_RECEIVED</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a06fc87d81c62e9abb8790b6e5713c55ba1fa663fc3bae079f330e318f206201f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DMA_READ</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>adf764cbdea00d65edcd07bb9953ad2b7a2915fe5d78ee9ccf385bc90e032aaaf6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DMA_WRITE</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>adf764cbdea00d65edcd07bb9953ad2b7a7b131f1a056b52cb5dbca2960c1130fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NO_PAYLOAD</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a99fb83031ce9923c84392b4e92f956b5a9f7d44692f16a80456f2a484dc81cc7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WITH_PAYLOAD</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a99fb83031ce9923c84392b4e92f956b5ab6ae1072798cf1e543ee32158e3bae43</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_start</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>ad49e25c0f0359f6826d8addcebf1e05c</anchor>
      <arglist>(sync_bool sync)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_start_paused</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>af41b39641c2de615ac5e006cf915202f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_exit</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>abe0417e22d07fa7bcdcfcc4ae29f7443</anchor>
      <arglist>(uint error)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_set_timer_tick_and_phase</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a16c5bdcb187e921a57c9b2849fd2e909</anchor>
      <arglist>(uint time, uint phase)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>spin1_set_timer_tick</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>aa6ab081b51fa79e0aac67a670f4c2873</anchor>
      <arglist>(uint time)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_get_simulation_time</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a79fba089ee6503e4c94648e0a2b3af10</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_pause</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a11a9fb2774fdaf83832fa9d37752df8d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_resume</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a788b5c55fa3faebafb5025bb858f76c4</anchor>
      <arglist>(sync_bool sync)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_rte</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a546b4f48774a886ed2302adf30bb4600</anchor>
      <arglist>(rte_code code)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>spin1_delay_us</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a96f68f8502ba29b4cf07c3023fc0dc03</anchor>
      <arglist>(uint n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_callback_on</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>afc98f97be297e7077334cad216d32b57</anchor>
      <arglist>(uint event_id, callback_t cback, int priority)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_callback_off</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>aaaad2f2f52dcfbd835c3928b5db97026</anchor>
      <arglist>(uint event_id)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_schedule_callback</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a653c1de1c4181f03c7ba4dbba38df9c7</anchor>
      <arglist>(callback_t cback, uint arg0, uint arg1, uint priority)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_trigger_user_event</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a19505c72d4618b8c00646d4dff1a328f</anchor>
      <arglist>(uint arg0, uint arg1)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_dma_transfer</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a1f4e5a5c1afbef1a0d3dafcfe96931a1</anchor>
      <arglist>(uint tag, void *system_address, void *tcm_address, uint direction, uint length)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_dma_flush</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a5032a57df544a2dbceb7d94cada28ffa</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_memcpy</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a8c23f19c28f2c3ed0a9b86ce8c98802d</anchor>
      <arglist>(void *dst, void const *src, uint len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_flush_rx_packet_queue</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a4703fab897e9d5409c1b569f1ace4729</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_flush_tx_packet_queue</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a77921134c64d1229235f89566f060a8c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_send_packet</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a0b239ad6683a630c2926321885395e27</anchor>
      <arglist>(uint key, uint data, uint TCR)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>spin1_send_mc_packet</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>acbf64a46f7c0a05a249d4a8c792ec53d</anchor>
      <arglist>(uint key, uint data, uint load)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>spin1_send_fr_packet</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a956c47dc4221ba53fe588a030f3cfe5e</anchor>
      <arglist>(uint key, uint data, uint load)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>spin1_msg_free</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a2f951eaa1b9ae4f71e0667c6144c7e92</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static sdp_msg_t *</type>
      <name>spin1_msg_get</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>ad757eba539d1f3ca7f40c39e9126e655</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>spin1_send_sdp_msg</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>abd057783076f2f7bdd7a7d53fd425140</anchor>
      <arglist>(sdp_msg_t *msg, uint timeout)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_irq_disable</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a693d489f90d16588a631a70ee6c3644c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_fiq_disable</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a56ccfdd40a5726c31fb1ee384e61649a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_int_disable</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a9037d19099fc2e2fbedaf0cce83261ef</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_mode_restore</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>aeaf770f6d983cd9d36cb88f09a7e8dd1</anchor>
      <arglist>(uint value)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_get_id</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a337ee4169620e7a86f8a81b81bb4f510</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_get_core_id</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>acc208cdfe1d4eb5caa298a8a5b4db50c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_get_chip_id</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>acb213e76a5a4d721b631082f30de45ad</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void *</type>
      <name>spin1_malloc</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>affff3f0d4e0978c5695098d0016f3b66</anchor>
      <arglist>(uint bytes)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>spin1_led_control</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>af8fd8b1292bfa89883067e5d263a9787</anchor>
      <arglist>(uint p)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>spin1_srand</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>abf21fb92f229f1a05fe4ab52e2913e3b</anchor>
      <arglist>(uint seed)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>spin1_rand</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a30d7b4286fc359f61944e7d499016ba1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>diagnostics_t</type>
      <name>diagnostics</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a4925eaa2ab3b95719d3af2c529e1d63b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>leadAp</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a8754c80284d361398ba776362765b92b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>spin1_api_cpp.h</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/include/</path>
    <filename>spin1__api__cpp_8h.html</filename>
    <includes id="spin1__api_8h" name="spin1_api.h" local="no" imported="no">spin1_api.h</includes>
  </compound>
  <compound kind="file">
    <name>spin1_api_params.h</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/include/</path>
    <filename>spin1__api__params_8h.html</filename>
    <class kind="struct">copy_t</class>
    <class kind="struct">dma_queue_t</class>
    <class kind="struct">packet_t</class>
    <class kind="struct">tx_packet_queue_t</class>
    <class kind="struct">user_event_t</class>
    <class kind="struct">user_event_queue_t</class>
    <class kind="struct">cback_t</class>
    <class kind="struct">task_t</class>
    <class kind="struct">task_queue_t</class>
    <member kind="define">
      <type>#define</type>
      <name>API_WARN</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a4efde7d7005762c053c3cbf300a4d7c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>API_DEBUG</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a04dc7cc998776500c6c07b123f836e80</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>API_DIAGNOSTICS</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a01426c0d3ec859e13276f2927c7d5ebf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NON_ROOT</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a26c31bf9a4f87dc346a7516b73d3d9e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>API_PRINT_DLY</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a65fc42bab17a581b66c46c8319db873d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_BURST_SIZE</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a1770d428fd0221aabcda7aad57a9b586</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_WIDTH</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>ad0b14f1d4cda14aea7d53c987af502b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_QUEUE_SIZE</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a10e1679d6cbffd931cff4a59e742ce51</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USE_WRITE_BUFFER</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>af4f8defb7a95fd463878a47036f0c572</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TX_PACKET_QUEUE_SIZE</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>affdc2771823cfd6bbd6d7722af52fc60</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TX_TCR_MCDEFAULT</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>aed27e951beff3526b1fda3db1a99205d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TX_FULL_MASK</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a08168d9862022c86f84ecb1afaa9d0c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TX_EMPTY_MASK</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>acb5c29649337b14c5d6acafa13e18b98</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RX_RECEIVED_MASK</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a58bf8c4718ce52b2f2084e0f39c59497</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USER_EVENT_QUEUE_SIZE</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>adc066f8781292318b521fc5eae90ce8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTS_STACKS</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>aedafca4205cf95f52e0af6f8be5c9365</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHIP_ADDR</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>aa674ac8567e7590da745c6eb045e9508</anchor>
      <arglist>(x, y)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_ROUTE</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a2698f9f10b499c3ecd68475cd3364c1b</anchor>
      <arglist>(addr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CORE_ROUTE</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a00b0a9eab12357f116b5466900bc26c3</anchor>
      <arglist>(core)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>isr_t</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a6d626fdf464bc6f2160d0bc92243b027</anchor>
      <arglist>)(void)</arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>spin1_api_link_orientations</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a636e84d914a4e0696012d9211152dc4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>spin1_api_error_codes</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a33401ad421a7aa22d725afe4dfc6d7e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>spin1_api_callback_queue_params</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a072100a7c6c61e4af40fc14936d87089</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NUM_PRIORITIES</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a072100a7c6c61e4af40fc14936d87089a67d35d8917ef5d8c7bc8730a936350f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TASK_QUEUE_SIZE</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a072100a7c6c61e4af40fc14936d87089a88f1a37f81d0995ede33ec78b87456df</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>spin1_api_multicast_entries</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a045642d971ae0a0fe08fefba8f50c1aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>spin1_api_vic_priorties</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a4a92b8ccb34182a85d66f54854bf0dd3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SARK_PRIORITY</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a4a92b8ccb34182a85d66f54854bf0dd3a7d8c0dc625f600c7e4979e9c9b0704f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TIMER1_PRIORITY</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a4a92b8ccb34182a85d66f54854bf0dd3a6b991ef752f76c445d5b25519ded8a6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DMA_DONE_PRIORITY</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a4a92b8ccb34182a85d66f54854bf0dd3a3fa68680e00066009a6de4e8c32fff90</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RX_READY_PRIORITY</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a4a92b8ccb34182a85d66f54854bf0dd3a22fac208f09583f8cb2c9def080da318</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FR_READY_PRIORITY</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a4a92b8ccb34182a85d66f54854bf0dd3aa7901f8d528b74129c56f18c6e279db0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CC_TMT_PRIORITY</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a4a92b8ccb34182a85d66f54854bf0dd3a25457f990e9b03a51bfc7c63d22dbfd5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SOFT_INT_PRIORITY</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a4a92b8ccb34182a85d66f54854bf0dd3a392dc30d79acd01884cafdbafe1dfb89</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DMA_ERR_PRIORITY</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a4a92b8ccb34182a85d66f54854bf0dd3a9d90bd4ab30fd8246c8399db33582f3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>cback_t</type>
      <name>callback</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a7f4271a2bb359cefd04d8ca74346ca05</anchor>
      <arglist>[NUM_EVENTS]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile isr_t *const</type>
      <name>vic_vectors</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a3cb5354368e271f2334fafd0ff932cc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>vic_controls</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a41e20072603ceef5db191a3c9f886570</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>spinnaker.h</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/include/</path>
    <filename>spinnaker_8h.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>INT_AT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6c34edc86f12feda2401c487739d6556</anchor>
      <arglist>(t)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NULL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a070d2ce7b6bb7e5c05602aa8c308d0c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BIT_31</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac397885e78e2ad51c518b95195a2a57e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BUF_WRITE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aee47e2374e5f2fb321540bbc086ecae0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NUM_CPUS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa061b51fb2d77d7721a3882edf5cd84a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NUM_LINKS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>adff16396e5bb9ba24b129ec2ab874031</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ITCM_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3e9685d681845b82fc822fcbe3d7fa74</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ITCM_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab79b8094d55a73bf2c7f75acc018f114</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ITCM_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7c0c967b2dbb7d7d7cc4c1c45e631816</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DTCM_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a82703357c25092e9944e110da68f02b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DTCM_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa3221e892c3f05fe2e2087d2d1a407db</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DTCM_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af73b85cd4e8cc6689b9a472823390294</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SDRAM_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5caf9d017993945f2d53d2af40fb4fec</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SDRAM_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3e88e07bff9f95a7d6c8828e4d03a388</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SDRAM_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac6cc132bcb1708323f4ddf973a618e06</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SDRAM_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a82db1eeff764aa7e568483199a0832b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SDRAM_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3327e481f6b201f76b17e9079ca574a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSRAM_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a550996465acbebce70d730c9df9ac717</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSRAM_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af8c78def245dc5dde56cbc4352ac9855</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSRAM_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2292f2ea539bb564b691a1a708447edf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSRAM_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2b1b50b8b83749d566ad6718f9ebd0b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSRAM_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae91b1725f67078b99e68cc3508d7f2e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ROM_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8a652cff0033969443cc2f6f5389fbd9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ROM_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad9e53d1418399b36953a40395ac1384d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ROM_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af05d8e99fa18f7d2c58191eac337a111</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a66e875a2bbefd558d6759f964ea0ca42</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a91c4d8a0aa1bf676c0b4d42ff50dd216</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6d396652b847faad2ff900a1c6870d11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_TCR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a292c556c2b3c038156a5febe8ea400f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_TXDATA</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a382fa8bcce043ad677c48badd0a54aa5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_TXKEY</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a641b07dbc5939a3c5ac455e4fc82dcf6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_RSR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6efa094ed1d22c2dfdea027236e0a6fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_RXDATA</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6a92dfc6ad00950c72ffdbc431b8e63b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_RXKEY</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a86f48eba76019f17309507fa46c8572f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_SAR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a40058952e8b62e51ade27b7a2c64efd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_TEST</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab2110a1e4e1b9bd89847b7a495765f2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2bb46b738c327b8a6373f53f1455bd5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae0ef987d1f3f7894f4892dbb0fdfd18f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a251f8c6600afee0dddf950c7a41d4723</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER_OFFSET</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a25ff717723cc5260b8a6988cb127a4da</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER1_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7a4bd01d91a70285f0bec70f4e9e88bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER2_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a933376d74e94dae8f42e17c09bd91faa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac247d87a6d8fc8d848154c7ed8978540</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_COUNT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7970f494b21987aaf86b9c1309e7f677</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_CONTROL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a70b8002a3a7b71bfd833e00903b3eaee</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_INT_CLR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa557f28f8ce0801c93aa3cfb5dbb4fd9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_RAW_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6b1a859d158740d5ed0880a28288fd48</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_MASK_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0dc46d281b9e66979b6705d2a542023b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_BG_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af2944984cc88d76db0a49c5c3c91093e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afaa9ce9bdbac4155e5d9b0a2009b9bce</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_COUNT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2af81def3304cbcdcd38221bf503ff56</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_CONTROL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a521657eaf0835d8cc3850452534342d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_INT_CLR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a48b700648448da29c15365d8a2495222</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_RAW_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae02c0c54c35664475c8d8b20bfed7b6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_MASK_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4e3c8e5979fef6b595acbb53c32cf874</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_BG_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>adab039400ff067abcd7b42bc18f0efbe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac5a2ce314a56ce61d69b13c6de9a228c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_COUNT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a03159a5a179d1f997653917ea8fddf02</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_CONTROL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7a9042ef65fb01d4c4ec7f2d8aed027b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_INT_CLR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afa4e55571473aa980d5446c98d5fd311</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_RAW_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a83a8b6d757ab73e6a830e8b40fe142a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_MASK_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a1248fc26bcead0dd4b1d466c6e5fc36a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_BG_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a87a2108658503d4c1c0cb0d4e211c8b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af845606e960afe9dd2e3bb618f3994aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a02d2c3987356c303a52aaaf334ccc209</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab04dd812f37907dc8bd6ed82e346b563</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_ADRS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa528cf5c140dfd48cc65353b56c8cdfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_ADRT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a76947a3b640ed9e5af8d10c976e4df86</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_DESC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0f5b8660a983c13c36078e0bd13b0bd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_CTRL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8a7d92a4f5077c21611200903b7fe8b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a973e8dd960fcf08a27c93600ad1bedc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_GCTL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a29edd3b5eaf2f9245fe58fa2ee24bb5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_CRCC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a53a96f1e4d562d25aae44068229036b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_CRCR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9385e244088e0f102b2524141fdf9bc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_TMTV</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a86051f7932a8c7a27a240b2c8239c9b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_SCTL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7d3734d31d30a51db5fe242b7637dcb4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9e6b324bf3a00c5581688b4173517a3d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>abf938bd6f1f6bc6b34915b7e755053b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af3d0a16e2925cbc3b0b76fa7a43aae91</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aeb8401ce4affb5679771e79f6b9da656</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT4</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aee9b50cee7f6bd94efde371810717c04</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT5</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7d756c077f6f83c4c08b41dd27200ca1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT6</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac1b9f8e3db93dddca274ae05063420b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT7</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9570d31c11c35e23c96af13ef373118a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_AD2S</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a692478015a3223180f034a63a354048d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_AD2T</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9537ce657a81b491836ce9e604f224a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_DES2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8e24032c93a69d33a7dbcdeb2f9dad19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_CRCT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>acd80bc7ddda5a5394971881da9c0a544</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PL340_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5532bfc36adb5a06962c91ef1548f22b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PL340_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a237a2e6d9a24fbd9b7d580bdef11629b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PL340_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aadb4b89bf854a4f26f6c9c37b87e8bb7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_STAT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa2fb5f05777d397c42ca3005475ae934</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CMD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6ed94d30762f635b033570a05bad8944</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_DIRC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a96e55d547b715e304616e00bd55b7114</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_MCFG</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2ac3acd51cd68ecf61b2357cc98e5576</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_REFP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7fbd4c3fae1da6aec3231d048ad27fcc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CASL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ade52e15ff2d5c434c81d3952a2fc6dcb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_DQSS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a11600ced164f728634acffad8c478d34</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_MRD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a71df53245336a8f9e49370e0313e2b2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RAS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7cd05b13608ba44a4681a829cfbf981a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a928bd5e6092d2a554b7be0b1c17f7d1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RCD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a37ddb3090a653a1ff74e9620fba0024a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RFC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aec04fc7df6bd3bb758ffdb197604406a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa75bf11f51828de9697993e95619e19a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RRD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa95b7aaf0f290a99bb4daea8ae06f921</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_WR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>add467cbb9768a39032fd52477f4a8e9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_WTR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4969473732f27dd0dace93a8639e6a54</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_XP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afcc9bf9da7a4bac60ddb344929239d5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_XSR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a42ba8ec7be45d732eda0a3f01969254b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_ESR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a26d79d8c30f969eb67a698ca4b9d4adf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_MCFG2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4a32b780504f4d58fcdb91e36218606d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_MCFG3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af40ae31777ef1051b4e15f7699107946</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0bbd7cb221196df8db33e18e2aa4a76d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7ac18f67e3dbc2bc8c049a558ef463ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af2f94140dbaadd4ba0bcd27da4249207</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a68fc3ae7dd005e70594dd7b7a85f72d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS4</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afedfe5030ca87a6fdcf221d7b2f315e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS5</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>acff5322b4ab2e5fc4153d3e54de7b558</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS6</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a93406b3a5a440237e9c946ab9c44900e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS7</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a374ee823d3b01f7fd5e9f07bc9b2d4bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS8</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5a1fb8746d8f47aa925eab6cdedb36ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS9</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af052cb335a52d9724ddba75c513e57d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS10</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac7c47dd229f81325ac7db0700d59b216</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS11</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a91da39b46884259ed8ce53c882856d28</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS12</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2bb8fcb165af9c96205a9c82c2425ee7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS13</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a35bae3abd3712940c0226b326a3f25ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS14</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a582d3d5ecb31b799bd3da770a2a65889</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS15</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8bacbe1dc3061b823d98183ea15b6c9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CCFG0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a891ab1868ecc8b20146423bdeeae8743</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CCFG1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a72f6112826e0a5e98dc437b1f292c88f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CCFG2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a57fd6ce53afce5586f3dd562ff8e1197</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CCFG3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5285d7bec993d8afa285452a72c7af5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLL_STATUS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae2d48a5e779c7483ae1d9aacca595d19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLL_CONFIG0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5c3c230b9cd641d42034f0a29ad1c06a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLL_CONFIG1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a358584d2afe3c33f60fc7a6f0fc933e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a279077ccdc8f031aff48b6be76bc1943</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a99151c52697d786535c570dc39dbb0fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af77649888b967f7daf3d976c4ae6e8a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_CONTROL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a72b46a26f477b393d69889a1f6ac961e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_STATUS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>acd551b99789b03e1cc571e9483d7a576</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_EHDR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ace2cf65fe13385efe970ade2c5d1eb4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_EKEY</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2327eef596894a19ce27055c5d7cdc26</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_EDAT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a21128f29654ab50544ba7b70ae67090f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_ESTAT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af6e350f6e9f3b96d56a797c9202e320c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DHDR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8c16c1cf5c39e22e11b7c4b25cdc9fd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DKEY</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab145a2921955b3d2b7400c9f42e0744a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DDAT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9109f3e7263ba26bd5e80c9f33943fd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DLINK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6479e19b881f6f7f748b53668317d126</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DSTAT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a38febfb02157e3965d75021c8c9cabe5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGEN</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab13acde74ab5be0317d334fec54b4618</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_FR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3aad90a71016cd5f82c64f8f5726fe6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7d331db7e25adddcb4734b55e009f71f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4f449daa404e5a2f3b02b989d382284b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aed9ef163f687af5ffaed82d91f4db9a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab8137295a1408e662950b88b32baeb3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF4</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0085598d35b92dba82ca69565388bf4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF5</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5d281c9d03ab0259ce610bf81285a575</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF6</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a19723eeea1ce1bf618d301cb5d74898a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF7</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0e7b383917d2a4bbb0eefca72726c01b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF8</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae2489c2470f74d90f7539fe84b1d1623</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF9</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae97e347b6d4d9c17707433ff0f99f633</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF10</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a208414e5c236322c44108280815c4aaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF11</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab7e53387f425ad70e509c0a354b532df</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF12</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0808bba52ef24660fcf6e05192058daf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF13</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab95abbc2705872f3b39772a1d3bfc2cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF14</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af5a261de465e63117ff2b2801c364277</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF15</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0db510fcb4f1c3a81175ebcbe3235af2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>adda97a9d124fae925277e3cbd6711f11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae57b4b02c1e7478af8a3b8bc30b105b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3b32cf60978e0d7b3f519fd42df3286c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4f4a28d6cad5aa2d9d34e34ca71edb58</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC4</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a129ee473d39f71b7457d221013b7a73f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC5</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae872b2ea024898f4358688f8401af7fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC6</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae659f73afd1fbb966ba6fdfc915697af</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC7</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a50820e4006639e3f3a61315788d05ccb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC8</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aaed81aed0f52a3967f75b7f17635b3bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC9</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a183b516f322c74b490e4be6ff7128758</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC10</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5d84c8b446ecfcd9e0be40d70cbeaef9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC11</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5daec9b386b2381c27a6ba7b033424c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC12</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac6a80ad65e04fbef08d0b3f469d2660d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC13</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6bec6cb9f13c61c8cf1599c4636b3326</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC14</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac7969732766a4f093e7c9779152350ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC15</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4f559822e9840ff11227ec5cd73bf506</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_TST1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3ad730be836ea3dc4796f4f096741bc4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_TST2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4853bd51ba21419f8e3210b95084e9b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_TABLE_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5e6bebe007e117b3d399252ede5d4933</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_TABLE_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa4a0b1604e13b1dc372f9c1bc8bf04a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RAM_WIDTH</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a99de5b422263f0ab019974c73061ee5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RAM_MASK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>adf72de763fed05b4b4e9d7b0f70b28ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CORE_ROUTE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab9d8a887acddae1c57b322d38ef22c83</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_LINK_ROUTE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8eb639051cd9708c8c99c55727461e0c</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_EPW</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac2a84572c9232da1ed33c06662bf3dcf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_LOG_EPW</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>acae32233aabafc649b6e8bf7b1e13cdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_EMASK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa2bf8e72defcc35c93a7d12a0bac1fa5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_BPE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3c8eae6c7a59ab144a206bb98d62216d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_BMASK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa3f4339e822f4aab6499ba47ed448ba8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_INIT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a33dc1d72ba48e2c3e0480ff9d5dbae70</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_MCRAM_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6529c7d0bdf518ce4495014c7b7e3189</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_MCRAM_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8f7d697e6cbc352a494e24c51906e773</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_MCKEY_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a50743d52c19a1abc9afe909ff454ed2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_MCKEY_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a19db3c54219a7e5a91995aec0f36f325</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_MCMASK_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a88375ad69fd553f0363ae9f3196366e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_MCMASK_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad5afaffb7c7be3cc327aed40cc21a35e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_P2P_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2613d82c442028398bc9e665bd195eff</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_P2P_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a52d73646ecc7b1f295711b5dc24ad059</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CAM_WIDTH</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>acec2c692ee0ecd2da8e77921b3828972</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CLK_XTAL_MHZ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8c6ee8c037bcef7942ace48de8414d51</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLL_260</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0d9d3a86305d5e8c3377f8edf42bfc0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLL_300</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab693709e8066de6b7fc3561569400ae2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLL_330</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5e0d4141a038e8bd796589cfedd194fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLL_400</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5144b0ea04e8fc306f41e92e561dcd92</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLL_LOCK_TIME</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a60c4cc679a24578f8b49cefb81caa79d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLL_CLK_SEL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a784356be8d9e0ecfdb3e33f0845ea2e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_USER</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9e3cdd5c5fb80a5739fa689c09f997a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_FIQ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2d95aed11b5420eb0d67884b39e984f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_IRQ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae3e562974dac9a93ece87414925c8c7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_SVC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9266f9bd89f1bccd0b3b5e8825ab654c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_ABT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a75f8757e1d238418b64ef59fc09e5e94</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_UND</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aade1410afb684e0540924f289ba68f23</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_SYS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac5fe4951d03afe9d7e282e754b655f7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THUMB_BIT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afd8f65e0d3bba35e4ec0b099c2bcbb44</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IMASK_IRQ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa87e465adff61c2e2ab45ace3d20ca4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IMASK_FIQ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aeeee93b904be3a58723a61716075340e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IMASK_ALL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afb04c134e59281a4687411af657d2d5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned char</type>
      <name>uchar</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a65f85814a8290f9797005d3b28e7e5fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned short</type>
      <name>ushort</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab95f123a6c9bcfee6a343170ef8c5f69</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned int</type>
      <name>uint</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a91ad9478d81a7aaf2593e8d9c3d06a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned long long</type>
      <name>uint64</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a29940ae63ec06c9998bba873e25407ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_MC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2daa1c6504afe93e64fc037e0fd40a953d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_P2P</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da0704bc33dfc02d9d9a7053400ba857e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_NN</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da0fc4ceef8da6f24cb2fcd22ab44027fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_NND</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da31369d00def165ef038d29dfef5d4904</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_FR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da54bf8773896c72c04f35e36dbeab5835</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_PL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da45ca553db3028627d8bb08f598da5fb9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_MC_PL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da97f5fc74acb65d379be157c7fe910b4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_P2P_PL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da12127ab38852d36c777a377e371abc9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_NN_PL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da2133cd6753ada6d7f3265db211d8a2ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_FR_PL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da181eca3e130f31d7715df73aa490c40c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>cc</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4621d877e9a2cef0ab1a6d9090502fab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>tc</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a13a58f8d0e73592bf3d2ee39f99a5094</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>tc1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af8be2229d31c7a50148614590ca75254</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>tc2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>abb3198b9994bcbd03a0876133e4ffd39</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>vic</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a609d1502b980469022fb488c9991ac23</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>dma</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae2c1510eaebd88bec122b31fcf8c782b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>sc</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2656318c453d34c84fca7693fd96e076</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>rtr</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a755f1c94052c3ad3ae5a829c7a38ab59</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>er</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>accfcb0efb6254eba4c6d02feba7cb7cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>mc</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5ddc40f567e69ab64b239d75f4e96d39</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>wd</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad7267d107405c779d711005de09d6fac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>sdram</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3e6234d9fac5edfc6956aee3cf428dd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>sysram</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a14ae5403ceb006ab9cf8b4ad803dc377</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>rtr_ram</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab6823340fba1b6dad489bf16786f7bbd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>rtr_key</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0b50befcdb072e5aec94ecdc851d1057</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>rtr_mask</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a1cb92c435b3e4a4d30cd5d9e0343163f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>rtr_p2p</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5e6d5402b6fbadf9db3a20c844daaa97</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uchar *const</type>
      <name>eth_tx_ram</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa80e83bd2b63ac4638be463ea9b11957</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uchar *const</type>
      <name>eth_rx_ram</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2102f9bb5f6a23ec8ecafa41257bf24f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>eth_rx_desc</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa27fcd745ad4d00769de7ee98d620fca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ITCM_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3e9685d681845b82fc822fcbe3d7fa74</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ITCM_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab79b8094d55a73bf2c7f75acc018f114</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ITCM_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7c0c967b2dbb7d7d7cc4c1c45e631816</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DTCM_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a82703357c25092e9944e110da68f02b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DTCM_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa3221e892c3f05fe2e2087d2d1a407db</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DTCM_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af73b85cd4e8cc6689b9a472823390294</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SDRAM_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5caf9d017993945f2d53d2af40fb4fec</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SDRAM_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3e88e07bff9f95a7d6c8828e4d03a388</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SDRAM_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac6cc132bcb1708323f4ddf973a618e06</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SDRAM_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a82db1eeff764aa7e568483199a0832b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SDRAM_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3327e481f6b201f76b17e9079ca574a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSRAM_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a550996465acbebce70d730c9df9ac717</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSRAM_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af8c78def245dc5dde56cbc4352ac9855</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSRAM_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2292f2ea539bb564b691a1a708447edf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSRAM_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2b1b50b8b83749d566ad6718f9ebd0b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSRAM_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae91b1725f67078b99e68cc3508d7f2e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ROM_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8a652cff0033969443cc2f6f5389fbd9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ROM_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad9e53d1418399b36953a40395ac1384d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ROM_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af05d8e99fa18f7d2c58191eac337a111</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a66e875a2bbefd558d6759f964ea0ca42</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a91c4d8a0aa1bf676c0b4d42ff50dd216</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6d396652b847faad2ff900a1c6870d11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_TCR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a292c556c2b3c038156a5febe8ea400f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_TXDATA</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a382fa8bcce043ad677c48badd0a54aa5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_TXKEY</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a641b07dbc5939a3c5ac455e4fc82dcf6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_RSR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6efa094ed1d22c2dfdea027236e0a6fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_RXDATA</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6a92dfc6ad00950c72ffdbc431b8e63b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_RXKEY</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a86f48eba76019f17309507fa46c8572f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_SAR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a40058952e8b62e51ade27b7a2c64efd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CC_TEST</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab2110a1e4e1b9bd89847b7a495765f2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2bb46b738c327b8a6373f53f1455bd5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae0ef987d1f3f7894f4892dbb0fdfd18f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a251f8c6600afee0dddf950c7a41d4723</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER_OFFSET</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a25ff717723cc5260b8a6988cb127a4da</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER1_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7a4bd01d91a70285f0bec70f4e9e88bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIMER2_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a933376d74e94dae8f42e17c09bd91faa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac247d87a6d8fc8d848154c7ed8978540</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_COUNT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7970f494b21987aaf86b9c1309e7f677</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_CONTROL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a70b8002a3a7b71bfd833e00903b3eaee</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_INT_CLR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa557f28f8ce0801c93aa3cfb5dbb4fd9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_RAW_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6b1a859d158740d5ed0880a28288fd48</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_MASK_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0dc46d281b9e66979b6705d2a542023b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TC_BG_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af2944984cc88d76db0a49c5c3c91093e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afaa9ce9bdbac4155e5d9b0a2009b9bce</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_COUNT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2af81def3304cbcdcd38221bf503ff56</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_CONTROL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a521657eaf0835d8cc3850452534342d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_INT_CLR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a48b700648448da29c15365d8a2495222</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_RAW_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae02c0c54c35664475c8d8b20bfed7b6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_MASK_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4e3c8e5979fef6b595acbb53c32cf874</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T1_BG_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>adab039400ff067abcd7b42bc18f0efbe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac5a2ce314a56ce61d69b13c6de9a228c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_COUNT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a03159a5a179d1f997653917ea8fddf02</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_CONTROL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7a9042ef65fb01d4c4ec7f2d8aed027b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_INT_CLR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afa4e55571473aa980d5446c98d5fd311</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_RAW_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a83a8b6d757ab73e6a830e8b40fe142a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_MASK_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a1248fc26bcead0dd4b1d466c6e5fc36a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T2_BG_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a87a2108658503d4c1c0cb0d4e211c8b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0e49c78897478ecea0156fb55cb19bde</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9c5b4e50c45b44c8d16bedc5e8c64b80</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a20d127cee2705b7e0b86585d131c2380</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_IRQST</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab4abb1ecd56a9f63c762ab8e83f37783</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_FIQST</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad794ec64b900d7a4c83046e6a830cf75</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_RAW</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a67d4149413aa323232ab1b21afffa103</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_SELECT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a25734b6cfdc6c4237f9295e66229ed19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ENABLE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab786e5132d4c90f83d87ad6ae16442e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_DISABLE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a446cf9d006b6b8774908b9c5d4e32579</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_SOFT_SET</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa3d8312fd36ea8f63b6bad81b0c39415</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_SOFT_CLR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af97c229a091e15b99b459af57082e1a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_PROTECT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>adad4a1df9cbb466c1246a02829e1d453</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_VADDR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4eaa7da4bb78e55fd0a6a201090f2b60</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_DEFADDR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a67819b77adc8b026a1cfa1d726ee3051</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac648788f351d2d23590450b58c428c13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5a502c9001aab604de12f7b0a8ff1a95</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6abf7fdb523d69327549a4a78cf91577</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae7ee6b7f38619ea67ed81e4d6d064417</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aed97fbcbbf6ea54eb26a162b6de4ba66</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR4</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a32cebab227a52263a19f361c933f7e54</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR5</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3aca9575db542eb53a5bd00b9b0193aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR6</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aedb9a79625f64ab9ccbe6a75167ff49a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR7</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a62b01b485993310020317068a6cd912f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR8</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a41173a2498204eccca7557be7e6721d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR9</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3d8b1a4d2a46e79530900be6cf232c5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR10</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aff148f14a4f298493116a82bb5932633</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR11</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab029c42116ef4d542c7d69ba290964e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR12</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a73db36be71f065eba8330c44a7ca02ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR13</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6f534f95a9cd26eceb16e3aa2f34b482</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR14</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae25b5640e316f74624b66951f7488615</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ADDR15</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa48fcc42846507ac2c780dbb0ffd222b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af37901349266284d10c9934b1d6eabb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a814b656b4d56e59deda245836ce8174d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5229c693a9f24047f6a847ab12c7c4bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a1e1389d97722ee87dd9596c22421aaf3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a17b0c84338f977365e30751413549179</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL4</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ade5d3f96bd3646d81063ede9a6f8a201</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL5</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2aec50c4ceebd55a9da9c044904b7f05</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL6</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad8fb2acd8f9d68e36857b8595bbe4798</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL7</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8fea3293d824f178bd352a1acd86e5dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL8</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a1ffd9216c320eb821bf9546c2c40d0c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL9</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa4d5ebdf6acfab95f750151318a198df</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL10</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae1bede8a23b4abb5c7f02542f949f3e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL11</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3eeb1f7b9871ce1007cab93b3d133e9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL12</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a1c15463a4f1c56287f9ef51215263feb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL13</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad2b3a65430edfbf5b8a3591b308f93cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL14</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac9e63c391b1d925bed5970720793b919</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_CNTL15</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a092e91b041741e612f0f89eaf9380a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>spinnaker_interrupt_numbers_e</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WDOG_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600caf8295a36f838b3798340f5eec0fe56e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SOFTWARE_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca17487a178a53e1db4d365f9a55e2fed2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>COMM_RX_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca517e52907c0a45f80d27e7958f70b6a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>COMM_TX_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca9690a28a16861d65186e5fa58ea0f97f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TIMER1_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600caeabd981f4a38bfe12277b8e77d20c847</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TIMER2_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca88797f8ba04c5f7f89ed22a011cbc9b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CC_RDY_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca3e75a2dcab406babe49448eacfcff125</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CC_RPE_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600cabac2b6e63814735f73376e8a5461cafa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CC_RFE_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca0cb6e364fca8af761eba8aa624a258f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CC_TFL_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca6d7985ff5a1f0e3bf952934bb8fbff5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CC_TOV_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca94e8bfd8a0af7be181d6cdf494e8963e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CC_TMT_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600cae34d1080356ee2dc50cf1e8a4788362a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DMA_DONE_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca7db46b6d08cd7733f042fb63832749b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DMA_ERR_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600caa14a8fc3283e4d514bbdb8195cfac609</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DMA_TO_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca34dbcc1a2c7136e62e1a6e268454d576</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTR_DIAG_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca905219a3073ce7e373df1106ad9f50b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTR_DUMP_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca2bf57b0c55a231fcce17000805e726e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RTR_ERR_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca9c1dc59c5d221c533e9bfe182d7f2249</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CPU_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600caa5158ef74d395e9c0379cef734a832b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ETH_TX_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca5a56ef2e819f95ee8664d2f23d181e5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ETH_RX_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600cab5d73c28cec81ae444dac81d33682dd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ETH_PHY_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600cad28601ef1735e5bff899e10505f73df6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SLOW_CLK_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600cad609ce48af9235d62828a7e4aa782807</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CC_TNF_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca5c6c067585bf6afa292dfbfef779fd5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CC_MC_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca98eb3a6f9f7094c28d8eed0aa4800e9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CC_P2P_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600caf1b4acf480fbc528819e63eab2bc2451</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CC_NN_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca54506dc30ec0736b40a45c886bbb2d87</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CC_FR_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca676247d31be7be8c3247c47fb697fdcf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INT0_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca0b797a8d4051fae405d5f59cc5a73c9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INT1_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca246ae3fdd04e987d1344116549158e3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>GPIO8_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600ca0999ab1a288b39954676a3d088bc004a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>GPIO9_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5b54d10e21fcb49a0672c0212179600caad0f6f0dc866cc9591e53f37af0a006e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af845606e960afe9dd2e3bb618f3994aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a02d2c3987356c303a52aaaf334ccc209</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab04dd812f37907dc8bd6ed82e346b563</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_ADRS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa528cf5c140dfd48cc65353b56c8cdfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_ADRT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a76947a3b640ed9e5af8d10c976e4df86</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_DESC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0f5b8660a983c13c36078e0bd13b0bd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_CTRL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8a7d92a4f5077c21611200903b7fe8b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a973e8dd960fcf08a27c93600ad1bedc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_GCTL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a29edd3b5eaf2f9245fe58fa2ee24bb5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_CRCC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a53a96f1e4d562d25aae44068229036b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_CRCR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9385e244088e0f102b2524141fdf9bc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_TMTV</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a86051f7932a8c7a27a240b2c8239c9b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_SCTL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7d3734d31d30a51db5fe242b7637dcb4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9e6b324bf3a00c5581688b4173517a3d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>abf938bd6f1f6bc6b34915b7e755053b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af3d0a16e2925cbc3b0b76fa7a43aae91</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aeb8401ce4affb5679771e79f6b9da656</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT4</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aee9b50cee7f6bd94efde371810717c04</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT5</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7d756c077f6f83c4c08b41dd27200ca1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT6</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac1b9f8e3db93dddca274ae05063420b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_STAT7</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9570d31c11c35e23c96af13ef373118a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_AD2S</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a692478015a3223180f034a63a354048d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_AD2T</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9537ce657a81b491836ce9e604f224a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_DES2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8e24032c93a69d33a7dbcdeb2f9dad19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DMA_CRCT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>acd80bc7ddda5a5394971881da9c0a544</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PL340_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5532bfc36adb5a06962c91ef1548f22b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PL340_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a237a2e6d9a24fbd9b7d580bdef11629b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PL340_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aadb4b89bf854a4f26f6c9c37b87e8bb7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_STAT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa2fb5f05777d397c42ca3005475ae934</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CMD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6ed94d30762f635b033570a05bad8944</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_DIRC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a96e55d547b715e304616e00bd55b7114</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_MCFG</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2ac3acd51cd68ecf61b2357cc98e5576</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_REFP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7fbd4c3fae1da6aec3231d048ad27fcc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CASL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ade52e15ff2d5c434c81d3952a2fc6dcb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_DQSS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a11600ced164f728634acffad8c478d34</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_MRD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a71df53245336a8f9e49370e0313e2b2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RAS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7cd05b13608ba44a4681a829cfbf981a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a928bd5e6092d2a554b7be0b1c17f7d1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RCD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a37ddb3090a653a1ff74e9620fba0024a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RFC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aec04fc7df6bd3bb758ffdb197604406a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa75bf11f51828de9697993e95619e19a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RRD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa95b7aaf0f290a99bb4daea8ae06f921</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_WR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>add467cbb9768a39032fd52477f4a8e9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_WTR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4969473732f27dd0dace93a8639e6a54</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_XP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afcc9bf9da7a4bac60ddb344929239d5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_XSR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a42ba8ec7be45d732eda0a3f01969254b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_ESR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a26d79d8c30f969eb67a698ca4b9d4adf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_MCFG2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4a32b780504f4d58fcdb91e36218606d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_MCFG3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af40ae31777ef1051b4e15f7699107946</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0bbd7cb221196df8db33e18e2aa4a76d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7ac18f67e3dbc2bc8c049a558ef463ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af2f94140dbaadd4ba0bcd27da4249207</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a68fc3ae7dd005e70594dd7b7a85f72d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS4</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afedfe5030ca87a6fdcf221d7b2f315e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS5</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>acff5322b4ab2e5fc4153d3e54de7b558</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS6</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a93406b3a5a440237e9c946ab9c44900e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS7</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a374ee823d3b01f7fd5e9f07bc9b2d4bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS8</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5a1fb8746d8f47aa925eab6cdedb36ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS9</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af052cb335a52d9724ddba75c513e57d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS10</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac7c47dd229f81325ac7db0700d59b216</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS11</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a91da39b46884259ed8ce53c882856d28</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS12</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2bb8fcb165af9c96205a9c82c2425ee7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS13</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a35bae3abd3712940c0226b326a3f25ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS14</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a582d3d5ecb31b799bd3da770a2a65889</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_QOS15</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8bacbe1dc3061b823d98183ea15b6c9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CCFG0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a891ab1868ecc8b20146423bdeeae8743</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CCFG1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a72f6112826e0a5e98dc437b1f292c88f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CCFG2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a57fd6ce53afce5586f3dd562ff8e1197</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CCFG3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5285d7bec993d8afa285452a72c7af5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLL_STATUS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae2d48a5e779c7483ae1d9aacca595d19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLL_CONFIG0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5c3c230b9cd641d42034f0a29ad1c06a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLL_CONFIG1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a358584d2afe3c33f60fc7a6f0fc933e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a279077ccdc8f031aff48b6be76bc1943</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a99151c52697d786535c570dc39dbb0fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af77649888b967f7daf3d976c4ae6e8a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_CONTROL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a72b46a26f477b393d69889a1f6ac961e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_STATUS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>acd551b99789b03e1cc571e9483d7a576</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_EHDR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ace2cf65fe13385efe970ade2c5d1eb4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_EKEY</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2327eef596894a19ce27055c5d7cdc26</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_EDAT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a21128f29654ab50544ba7b70ae67090f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_ESTAT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af6e350f6e9f3b96d56a797c9202e320c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DHDR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8c16c1cf5c39e22e11b7c4b25cdc9fd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DKEY</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab145a2921955b3d2b7400c9f42e0744a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DDAT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9109f3e7263ba26bd5e80c9f33943fd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DLINK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6479e19b881f6f7f748b53668317d126</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DSTAT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a38febfb02157e3965d75021c8c9cabe5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGEN</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab13acde74ab5be0317d334fec54b4618</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_FR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3aad90a71016cd5f82c64f8f5726fe6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7d331db7e25adddcb4734b55e009f71f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4f449daa404e5a2f3b02b989d382284b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aed9ef163f687af5ffaed82d91f4db9a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab8137295a1408e662950b88b32baeb3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF4</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0085598d35b92dba82ca69565388bf4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF5</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5d281c9d03ab0259ce610bf81285a575</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF6</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a19723eeea1ce1bf618d301cb5d74898a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF7</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0e7b383917d2a4bbb0eefca72726c01b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF8</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae2489c2470f74d90f7539fe84b1d1623</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF9</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae97e347b6d4d9c17707433ff0f99f633</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF10</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a208414e5c236322c44108280815c4aaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF11</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab7e53387f425ad70e509c0a354b532df</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF12</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0808bba52ef24660fcf6e05192058daf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF13</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab95abbc2705872f3b39772a1d3bfc2cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF14</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af5a261de465e63117ff2b2801c364277</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGF15</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0db510fcb4f1c3a81175ebcbe3235af2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>adda97a9d124fae925277e3cbd6711f11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae57b4b02c1e7478af8a3b8bc30b105b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3b32cf60978e0d7b3f519fd42df3286c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC3</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4f4a28d6cad5aa2d9d34e34ca71edb58</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC4</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a129ee473d39f71b7457d221013b7a73f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC5</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae872b2ea024898f4358688f8401af7fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC6</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae659f73afd1fbb966ba6fdfc915697af</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC7</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a50820e4006639e3f3a61315788d05ccb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC8</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aaed81aed0f52a3967f75b7f17635b3bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC9</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a183b516f322c74b490e4be6ff7128758</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC10</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5d84c8b446ecfcd9e0be40d70cbeaef9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC11</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5daec9b386b2381c27a6ba7b033424c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC12</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac6a80ad65e04fbef08d0b3f469d2660d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC13</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6bec6cb9f13c61c8cf1599c4636b3326</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC14</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac7969732766a4f093e7c9779152350ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_DGC15</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4f559822e9840ff11227ec5cd73bf506</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_TST1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3ad730be836ea3dc4796f4f096741bc4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_TST2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4853bd51ba21419f8e3210b95084e9b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_TABLE_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5e6bebe007e117b3d399252ede5d4933</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_TABLE_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa4a0b1604e13b1dc372f9c1bc8bf04a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RAM_WIDTH</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a99de5b422263f0ab019974c73061ee5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_RAM_MASK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>adf72de763fed05b4b4e9d7b0f70b28ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CORE_ROUTE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab9d8a887acddae1c57b322d38ef22c83</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_LINK_ROUTE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8eb639051cd9708c8c99c55727461e0c</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_EPW</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac2a84572c9232da1ed33c06662bf3dcf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_LOG_EPW</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>acae32233aabafc649b6e8bf7b1e13cdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_EMASK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa2bf8e72defcc35c93a7d12a0bac1fa5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_BPE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3c8eae6c7a59ab144a206bb98d62216d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_BMASK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa3f4339e822f4aab6499ba47ed448ba8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_INIT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a33dc1d72ba48e2c3e0480ff9d5dbae70</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_MCRAM_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6529c7d0bdf518ce4495014c7b7e3189</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_MCRAM_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8f7d697e6cbc352a494e24c51906e773</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_MCKEY_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a50743d52c19a1abc9afe909ff454ed2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_MCKEY_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a19db3c54219a7e5a91995aec0f36f325</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_MCMASK_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a88375ad69fd553f0363ae9f3196366e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_MCMASK_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad5afaffb7c7be3cc327aed40cc21a35e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_P2P_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2613d82c442028398bc9e665bd195eff</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RTR_P2P_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a52d73646ecc7b1f295711b5dc24ad059</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MC_CAM_WIDTH</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>acec2c692ee0ecd2da8e77921b3828972</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSCTL_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae086e501ae842808936bba2d5b161317</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSCTL_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a26a58f6ccd3e260f9a1c38ea4d6018d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SYSCTL_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a568834119b9aae01b7f69e8a012d3bf9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_CODE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a29f9ed2d1eed6eae901b2e9f01d76f6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHIP_ID_CODE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4f0c819e4d06ff5928abeadfdc80f744</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_CHIP_ID</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9dc6ef961c15d88167f0ae6194529dd8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_CPU_DIS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a80250ce6ee4f570acb80371742e14fe8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_SET_IRQ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aec5cb88364cfb11614cf4e20197f25f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_CLR_IRQ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad04f045dbbb387420e1438442f7ddc19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_SET_OK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2193bf3caa2ce539a0664223ecd45ab0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_CPU_OK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad39f89551ac32b7613c56dec3490ff47</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_CLR_OK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a96a7eaf7cc6be3c48f5adf37538914fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_SOFT_RST_L</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a906f07319a780dcfb03f5c449444badd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_HARD_RST_L</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a083ca24ce593ef03a1d3e4889340c5df</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_SUBS_RST_L</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>acf8704f2883367cb09a1a8c5e068b2aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_SOFT_RST_P</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5c6ec5f6c9b336bfa2fc88b2e685be16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_HARD_RST_P</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0a3c79ba4b4be60d49115e9f65c6f8c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_SUBS_RST_P</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a563161ae55acc840b700d2300a612af0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_RST_CODE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>abeaf8d60c0822497066c55736fff7af8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_MON_ID</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5d69620cdde1874e1c4b5b4bcb1b8861</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_MISC_CTRL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab2264dafbf92b753217d962be69f8712</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GPIO_RES</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a91b0e8413909e40d7dd79270c596512f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GPIO_PORT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac5d7411fcdf1f2bb51fb57db9fe7c85c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GPIO_DIR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a637a20b456c93e8248c861fa48510841</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GPIO_SET</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aee1a56baeb6b8457fb722fd36dad5bbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GPIO_CLR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a41e1490709d11632c6584a5baef75d29</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GPIO_READ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a1a5b0b764215bbcab9c8c675ef55deea</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_PLL1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8451a2f05dc0771f7fb5c7d4c0143e2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_PLL2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a1cd60cb8d2561b88ccc32f9087c46dd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_FLAG</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a187d98ded6609d79de1f5d4e7f60f0ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_SETFLAG</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a12fbb23650043e51eea8b754ec4b0cf2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_CLRFLAG</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aba705ba3ac9cc166f4b99b6074ff4f4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_CLKMUX</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0bf4540d6be26ebaca1dbcfb219310aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_SLEEP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa6b265f20cf154bc41971e3eaba011d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_TS0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a6b18aae73f5bcdcd466ad55a0cb41bbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_TS1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7486b1722f74718073652977236317be</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_TS2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a337e95ab4215ceaad9fed279a7178e4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_ARB0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afa42cd4f712fba1137083da9b0ecb3f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_TAS0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a1ee2590c7f96165010beae2079f72150</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_TAC0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab743b17923c0ee552eaf10e73995a1ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SC_LINK_DIS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad9e21b61f9a8a638731d46ea08350be4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RST_POR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>abc6126af1d45847bc59afa0aa3216b04acd51f5dfabe63d05d5bc0105549d003e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RST_WDT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>abc6126af1d45847bc59afa0aa3216b04a40678c7575f6977727e7eebfb08e22d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RST_USER</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>abc6126af1d45847bc59afa0aa3216b04ab7ac2c8790c58e8592eb9afafb8aa289</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RST_SW</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>abc6126af1d45847bc59afa0aa3216b04a066003c79359a0b9bb25ea84ca4c9c6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RST_WDI</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>abc6126af1d45847bc59afa0aa3216b04a2a120bd4d5981c4aaa74f94367e34e91</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0c501c10f080f6b0741e4c94aaca0a4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac9e1a3c3d9c55a403fdcedce3ddc593b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0c092d77d4599871d1ebda1a3a28e887</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WD_LOAD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9a0b114619881b521cd57ee8b6016220</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WD_COUNT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa8aedd922a5452403bcb77f6b7dc145b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WD_CTRL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5403d692c4ce352fcff30a1f9e4e8a09</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WD_INTCLR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a97e875a2503e73ecf7b9ff4b83a636fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WD_RAWINT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa38bd290318420536652ec9f44e97c5e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WD_MSKINT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a41fc695eb84598dcb5c4fd9522a3257f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WD_LOCK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afbc7d020667d1b2520aae8d40e3653ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WD_CODE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a32b3a67691a686edef590b6dcffc3053</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WD_INT_B</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>adc29c2ff13d900c2f185ee95427fb06cafb377d5368a8e94baade54ef1120b779</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WD_RST_B</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>adc29c2ff13d900c2f185ee95427fb06ca760ce2764be2ece0e40d565e3834db3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_BASE_UNBUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac48d2518ff56767011fc63b484d59e51</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_BASE_BUF</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aadc8d7e7687f1157d474d757fa656c7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad965a7b1106ece575ed3da10c45c65cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_TX_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a727bb35e70df4254583e1f3f19a69548</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_TX_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afd2892e976fabdd304ff4a463e88ebde</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_TX_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aecd4fb7cb5fff682bf3d190cef5d9bf8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_RX_BASE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>acc27123d96b77e202948d11348beca38</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_RX_SIZE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5a3344783209669181456fb7dc84a594</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_RX_TOP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a498c974a2cfc360b666ef93578239656</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_RX_DESC_RAM</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a959d4d758ece436b258f7f7f0c9349cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_REGS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a839881b5d0c1b3b34ad98e667d0fec16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_CONTROL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac2b629f6a9591422ee96a547c2e0c1eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_STATUS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a242d406ac7501201b1bbefe7aa9bc6a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_TX_LEN</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a217d113342fcba3d46c06edbc59aaf1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_TX_CMD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af44d96b844b21dd26324e79cd2017277</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_RX_CMD</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac0c32800c327c46059e1ba3004423c43</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_MAC_LO</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae1bfa067f7f8980d88a5a5c01212c887</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_MAC_HI</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a792a45cf05601a0438d9356fcad990d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_PHY_CTRL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa60959bd55dfd39dedec51f55e892bd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_INT_CLR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a7774c16af94e7a76fe35d15959d4df37</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_RX_BUF_RP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a464e7f1ea120bf59b8073166ff347c51</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETH_RX_DESC_RP</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a16dffebfbd2ea6bf23e143ab233bb134</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ETH_TX_CLR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a61dadd085c1777f559549e05962b2c9ea63e81dc0e2a2f895ddce03f9ec777444</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ETH_RX_CLR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a61dadd085c1777f559549e05962b2c9ea2216f852b59ed6bc84b33dcea4d7041e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_CTRL_NRST</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a726ca809ffd3d67ab4b8476646f26635a64b3745325cd173842a83319abd4b5f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_CTRL_DIN</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a726ca809ffd3d67ab4b8476646f26635a34b4d1dd3b909ddb337288557da24486</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_CTRL_DOUT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a726ca809ffd3d67ab4b8476646f26635aac9b12dd447314487a2c20c475eb6ef8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_CTRL_OE</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a726ca809ffd3d67ab4b8476646f26635a531c8b43ca9a5df8842d31c6d29ff458</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_CTRL_CLK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a726ca809ffd3d67ab4b8476646f26635a7f7636c9175ad0bbe21ed6ad75fa2731</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>cc</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a4621d877e9a2cef0ab1a6d9090502fab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>tc</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a13a58f8d0e73592bf3d2ee39f99a5094</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>tc1</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>af8be2229d31c7a50148614590ca75254</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>tc2</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>abb3198b9994bcbd03a0876133e4ffd39</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>vic</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a609d1502b980469022fb488c9991ac23</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>dma</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae2c1510eaebd88bec122b31fcf8c782b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>sc</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2656318c453d34c84fca7693fd96e076</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>rtr</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a755f1c94052c3ad3ae5a829c7a38ab59</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>er</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>accfcb0efb6254eba4c6d02feba7cb7cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>mc</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5ddc40f567e69ab64b239d75f4e96d39</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint *const</type>
      <name>wd</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ad7267d107405c779d711005de09d6fac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>sdram</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a3e6234d9fac5edfc6956aee3cf428dd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>sysram</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a14ae5403ceb006ab9cf8b4ad803dc377</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>rtr_ram</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab6823340fba1b6dad489bf16786f7bbd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>rtr_key</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0b50befcdb072e5aec94ecdc851d1057</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>rtr_mask</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a1cb92c435b3e4a4d30cd5d9e0343163f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>rtr_p2p</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5e6d5402b6fbadf9db3a20c844daaa97</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uchar *const</type>
      <name>eth_tx_ram</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa80e83bd2b63ac4638be463ea9b11957</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uchar *const</type>
      <name>eth_rx_ram</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2102f9bb5f6a23ec8ecafa41257bf24f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint *const</type>
      <name>eth_rx_desc</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa27fcd745ad4d00769de7ee98d620fca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_MC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2daa1c6504afe93e64fc037e0fd40a953d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_P2P</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da0704bc33dfc02d9d9a7053400ba857e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_NN</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da0fc4ceef8da6f24cb2fcd22ab44027fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_NND</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da31369d00def165ef038d29dfef5d4904</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_FR</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da54bf8773896c72c04f35e36dbeab5835</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_PL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da45ca553db3028627d8bb08f598da5fb9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_MC_PL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da97f5fc74acb65d379be157c7fe910b4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_P2P_PL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da12127ab38852d36c777a377e371abc9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_NN_PL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da2133cd6753ada6d7f3265db211d8a2ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PKT_FR_PL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0411cd49bb5b71852cecd93bcbf0ca2da181eca3e130f31d7715df73aa490c40c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CLK_XTAL_MHZ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a8c6ee8c037bcef7942ace48de8414d51</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLL_260</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a0d9d3a86305d5e8c3377f8edf42bfc0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLL_300</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ab693709e8066de6b7fc3561569400ae2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLL_330</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5e0d4141a038e8bd796589cfedd194fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLL_400</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a5144b0ea04e8fc306f41e92e561dcd92</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLL_LOCK_TIME</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a60c4cc679a24578f8b49cefb81caa79d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLL_CLK_SEL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a784356be8d9e0ecfdb3e33f0845ea2e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOP_BIT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a374a89ea463741f80c7e7b9978664bc9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LED_0</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa94efefdb04d44c846c163f108d52ae9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>JTAG_RTCK</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a385c44f6fb256e5716a2302a5b940388ab8cc5ebc1d10f1e13020635c65f77eef</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>JTAG_TDO</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a385c44f6fb256e5716a2302a5b940388a2fe235d834ee0231648ebed23f100fb1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>JTAG_INT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a385c44f6fb256e5716a2302a5b940388a984fab6c48baa00cb5b17e7988097ba8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_USER</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9e3cdd5c5fb80a5739fa689c09f997a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_FIQ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a2d95aed11b5420eb0d67884b39e984f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_IRQ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ae3e562974dac9a93ece87414925c8c7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_SVC</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a9266f9bd89f1bccd0b3b5e8825ab654c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_ABT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>a75f8757e1d238418b64ef59fc09e5e94</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_UND</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aade1410afb684e0540924f289ba68f23</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODE_SYS</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>ac5fe4951d03afe9d7e282e754b655f7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THUMB_BIT</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afd8f65e0d3bba35e4ec0b099c2bcbb44</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IMASK_IRQ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aa87e465adff61c2e2ab45ace3d20ca4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IMASK_FIQ</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>aeeee93b904be3a58723a61716075340e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IMASK_ALL</name>
      <anchorfile>spinnaker_8h.html</anchorfile>
      <anchor>afb04c134e59281a4687411af657d2d5c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>version.h</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/include/</path>
    <filename>version_8h.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>SLLT_VER_STR</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>a48e1d0220cfb6e6b85ba6dfab5d456a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SLLT_VER_NUM</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>a1e0f0d41d60802c031eb36bd0e4342c0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sark_alloc.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/sark/</path>
    <filename>sark__alloc_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="no" imported="no">sark.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>FREE_MASK</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>a97bc89d16faea696822ecaedc7ea0094</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>sark_xalloc</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>a8d1f83786e86e8943b1e1a2d8fa68b85</anchor>
      <arglist>(heap_t *heap, uint size, uint tag, uint flag)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>sark_alloc</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>a30d1e6e1b258fa76d6412827a1aa4f15</anchor>
      <arglist>(uint count, uint size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_xfree</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>ae4289de17bb19977ccbc8b1f680b4a6a</anchor>
      <arglist>(heap_t *heap, void *ptr, uint flag)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_free</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>a40ce895f48f22b29d2aefc923b38b280</anchor>
      <arglist>(void *ptr)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_xfree_id</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>a0d1e347ad21b21b298040b27f36a2bba</anchor>
      <arglist>(heap_t *heap, uint app_id, uint lock)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_heap_max</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>a8e89c52519c74f83f002becac03d9ff3</anchor>
      <arglist>(heap_t *heap, uint flag)</arglist>
    </member>
    <member kind="function">
      <type>heap_t *</type>
      <name>sark_heap_init</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>ab76688949797930a80f58584467243c2</anchor>
      <arglist>(uint *base, uint *top)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_alloc_id</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>a2d7bb03f41c2c68ad7fdb944bb23af64</anchor>
      <arglist>(uint size, uint app_id)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_alloc</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>abb9d9cc3e46a707969eb592b258744e5</anchor>
      <arglist>(uint size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_free</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>a5cb6e9bbf8425f6fe224833f6239c4ba</anchor>
      <arglist>(uint entry, uint clear)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_free_id</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>a7e038cf666dba5f8be35bb8cb8eead45</anchor>
      <arglist>(uint app_id, uint clear)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_alloc_max</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>af89de816e4c181b8f24ba21873b24a27</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>sark_tag_ptr</name>
      <anchorfile>sark__alloc_8c.html</anchorfile>
      <anchor>a226f6ac4ac38c44dce34a617ed1f1aa6</anchor>
      <arglist>(uint tag, uint app_id)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sark_base.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/sark/</path>
    <filename>sark__base_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="no" imported="no">sark.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>SARK_ID_STR</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>ab5c0a12c6dfa3b12cc5cc5e9381538b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_int_han</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a6d542339b492b2ad62a803b09b001365</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_fiq_han</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>af04f90a416df87565db777e5a3191910</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rte_handler</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a949dafe255d21ee751e45be934f3f05f</anchor>
      <arglist>(uint code)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_chip_id</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>aa10c5c53ee9f467e3aad6e1a054cc466</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_core_id</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a57fab81f030fe93ea4f247f2d27f1e53</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_app_id</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a5a95df8505e42ee47d56f3bb752a8ddc</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_count_bits</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a2899892c855c4e5260c0b59086ad3866</anchor>
      <arglist>(uint word)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_str_len</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>aeef96491568c6a6468cb4d35d1d18d07</anchor>
      <arglist>(char *string)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_cpu_state</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a8c3cb686bab55b58b206f7f13b7ecafa</anchor>
      <arglist>(cpu_state state)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_app_raise</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>ad8d3315f83d10fc3071fc74ddc239248</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_app_lower</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>ad0488210f65d863dd3a609c71ae34a1b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_app_sema</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>ae711485b0ed4f29a98a08f4476e6c1ed</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_app_cores</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a7b765a16adc6f4a88ac9017b9202ba83</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_app_lead</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>affdba350f69ec3d46d9d559571feb0f6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sw_error_fl</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>aa82adc6677b15f452e59063cf4a8f2f0</anchor>
      <arglist>(sw_err_mode mode, char *file, uint line)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>sark_block_init</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>afa47ca11c3ee85b99d9ab9be58e8e082</anchor>
      <arglist>(void *buf, uint count, uint size)</arglist>
    </member>
    <member kind="function">
      <type>sdp_msg_t *</type>
      <name>sark_msg_get</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>ab3ab790eb60f2c24aa9b20fff884e64b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_msg_free</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a8b6bbbfa956f793ea41c8702879cffbd</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>sark_block_get</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a93d1de5c5e2f2a1607c05eb6f024a183</anchor>
      <arglist>(mem_block_t *root)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_block_free</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a3d40f5611a6a76a083d0751d2ffcebc3</anchor>
      <arglist>(mem_block_t *root, void *blk)</arglist>
    </member>
    <member kind="function">
      <type>sdp_msg_t *</type>
      <name>sark_shmsg_get</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a1bdfaf0e7bfe553bcafc6d04bf321bf1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_shmsg_free</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a69f3c808f1b7045354a386c2bd2872df</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_call_cpp_constructors</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a2885d11fa603c565e9084ffffc3fe557</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_init</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>ab496b0c84b642ec5c8cb65a2bd60cab1</anchor>
      <arglist>(uint *stack)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_pre_main</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a34ed2549430ecaaa7a6972ad7fb79434</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_post_main</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a752454711da73d1c9d31ccb8d1feafd2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_msg_send</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a4e20f66a6e7058940cdbfdd077fbcdc4</anchor>
      <arglist>(sdp_msg_t *msg, uint timeout)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_cmd_ver</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a10a6d37b96c4cd0339de66851e9852e1</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_cmd_read</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a05105db095cf54052c1e00cb2d03424b</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_cmd_write</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a214cec2f71f5dc818d88c0abc398fe29</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>sark_cmd_fill</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>af67d477747aa6b62a2448590a7f4ed2d</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>sark_debug</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a6bf88c2e52da133466773a2d58698625</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>swap_hdr</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a997d5a71dff5a1773459396fc09a5207</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_int</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a11cb3c4dcaa6cfbd7a2a8923443647ac</anchor>
      <arglist>(void *pc)</arglist>
    </member>
    <member kind="variable">
      <type>sark_data_t</type>
      <name>sark</name>
      <anchorfile>sark__base_8c.html</anchorfile>
      <anchor>a89aedee21d244fb4bf7613609168c7b4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sark_div0.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/sark/</path>
    <filename>sark__div0_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="no" imported="no">sark.h</includes>
    <member kind="function">
      <type>void</type>
      <name>__aeabi_idiv0</name>
      <anchorfile>sark__div0_8c.html</anchorfile>
      <anchor>a1e538d7c66fa8df178a5929d8c0f4b6c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>__aeabi_ldiv0</name>
      <anchorfile>sark__div0_8c.html</anchorfile>
      <anchor>a1b9cf9b01154938e9e9c2dc62f3fa25b</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sark_event.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/sark/</path>
    <filename>sark__event_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="no" imported="no">sark.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>EFAIL</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a02cfa0a6c71550c8857e2ad599c1fed2</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>efail_code</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a23769f9b0b769271cef0e19c1719e7ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EFAIL_FIQ</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a23769f9b0b769271cef0e19c1719e7caa5f2ad490bd428667ee256b1cf324190d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EFAIL_IRQ</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a23769f9b0b769271cef0e19c1719e7caa9d1df513fdb4cbb1a53bce1c76d668b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EFAIL_QUEUE</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a23769f9b0b769271cef0e19c1719e7caaa735f6ca002e6aebbf3ccfe3174f8199</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EFAIL_ALLOC</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a23769f9b0b769271cef0e19c1719e7caa7475b5fb7343c7d8fec997d262a8e424</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EFAIL_NEW</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a23769f9b0b769271cef0e19c1719e7caa0dda37d673d93fd7f6da18d27bfa25a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>event_user</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a0354578d007f0031b66bb376939aee54</anchor>
      <arglist>(uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_register_int</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a4ff795c1f197635ab02d6316001e8344</anchor>
      <arglist>(event_proc proc, event_type event_num, vic_slot slot)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_register_queue</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>ad79ef0712646ec5699bd9352d1c196bd</anchor>
      <arglist>(event_proc proc, event_type event_num, vic_slot slot, event_priority priority)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_enable</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>afc420e5045a73751d480bf2edfb15e72</anchor>
      <arglist>(event_type event_num, uint enable)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_register_pause</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a6c35de6a8f9072dc49db8984bd7ce135</anchor>
      <arglist>(event_proc proc, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_wait</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a8d885d4db63c2de6f5982e74e86cb84f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_run2</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a98bf1ef0674349b047c50de2e4fee790</anchor>
      <arglist>(uint restart)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>event_start</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>afa72805def2820951b0e8f1967d518fd</anchor>
      <arglist>(uint period, uint events, sync_bool wait)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_pause</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a65e02c560e39a7afafbb5da4255906f4</anchor>
      <arglist>(uint pause)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_stop</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a05b364e4b7263b9e979f4f5d02369fa3</anchor>
      <arglist>(uint rc)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>enqueue_event</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>afdd99d86f447eb094d17c9039852271f</anchor>
      <arglist>(event_t *e, event_priority priority)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>event_queue</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a607f8bbe9eb8a7d99a4a335be0673b09</anchor>
      <arglist>(event_t *e, event_priority priority)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>event_queue_proc</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a99da351948b0933e1a8f46db6d9b5f7d</anchor>
      <arglist>(event_proc proc, uint arg1, uint arg2, event_priority priority)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_free</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a05a1933cd3173bcfe01e10150532b933</anchor>
      <arglist>(event_t *e)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static event_t *</type>
      <name>get_queue_contents</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a995b81ab23ec1ff4b6442f6656fec157</anchor>
      <arglist>(proc_queue_t *queue)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_run</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a6c997d47394c965abda9a03d8db70ae6</anchor>
      <arglist>(uint restart)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static event_t *</type>
      <name>take_one_event_from_queue</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a9548bdb6b6a0f0a21daa68350693f5d5</anchor>
      <arglist>(proc_queue_t *queue)</arglist>
    </member>
    <member kind="function">
      <type>event_t *</type>
      <name>event_new</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>ac3dc841f217092d14cd043929c4ef5a0</anchor>
      <arglist>(event_proc proc, uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>event_alloc</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a06ace566279f8f98e6bf001dee74eeab</anchor>
      <arglist>(uint events)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_config</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a0ef7337b252506ac9e43cf185faf5ea9</anchor>
      <arglist>(event_t *e, event_proc proc, uint arg1, uint arg2)</arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>fiq_events</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a4bed7ec3e558e97cc6d51f8d6ded3569</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>irq_events</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a87c2954cb2c69d68488bbd1b4beea749</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>queue_events</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a8f2193917ee832f286bb6a34f18394b5</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>null_events</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>a3e2764d3a749b34fb9a36f883c8de5a4</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const uchar</type>
      <name>vic_bit</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>adb9a00f7735c95fa332c259695b951c1</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>event_data_t</type>
      <name>event</name>
      <anchorfile>sark__event_8c.html</anchorfile>
      <anchor>ae7a7108766201ba6bd821b143da63f09</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sark_hw.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/sark/</path>
    <filename>sark__hw_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="no" imported="no">sark.h</includes>
    <member kind="function">
      <type>void</type>
      <name>null_int_han</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a4f49b7b8f990fad416ad7a5f50585ccc</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>vic_error</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>adae7021b786cdfa94565a316acabbe32</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_vic_init</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>aed572d885f25c88acdc5a11c26e51b2c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_vic_set</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a8e5523f51f1df77463689f9567c87884</anchor>
      <arglist>(vic_slot slot, uint interrupt, uint enable, int_handler handler)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>v2p_mask</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a28c92abd55df0daf0b4b05284277c8b8</anchor>
      <arglist>(uint virt_mask)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>get4</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>aa3fc2d449c65d25d3151ce7d034d0249</anchor>
      <arglist>(const uint *word, uint index)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_led_init</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a7338a87d005d821ee1ef167eda592ecb</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_led_set</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a74a3a76206602b65fc8f482d0c9cce5d</anchor>
      <arglist>(uint leds)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pl340_init</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a5bfda3e690ab2344e88f6e8934e2edd4</anchor>
      <arglist>(uint mem_clk)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_mc_clear</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>afc00e275bcd21290b0ade7caffb3f690</anchor>
      <arglist>(uint start, uint count)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_mc_init</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a151645129faec74d427f2961d4ab9449</anchor>
      <arglist>(uint start)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_mc_load</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a3ba7cd3d8b44e6225eec84c46d20a441</anchor>
      <arglist>(rtr_entry_t *e, uint count, uint offset, uint app_id)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_mc_set</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a81ca61e30076d1724a68027b63f6e970</anchor>
      <arglist>(uint entry, uint key, uint mask, uint route)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_mc_get</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>acfe404b27b699071843bb9fbe10a5325</anchor>
      <arglist>(uint entry, rtr_entry_t *r)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_fr_set</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a71b1b5e0a5526750b671ebe436da92f3</anchor>
      <arglist>(uint route)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_fr_get</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>ab87c4fc234cb78333321bc0acf3b7b75</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_p2p_init</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a74ab37d7c02d8246d2b0fdc7670ce5fa</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_p2p_set</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a42ab52595bb1286fe7428cad3f2670d9</anchor>
      <arglist>(uint entry, uint value)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>rtr_p2p_get</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>ae43c60a896bbc207a45724881f051e6a</anchor>
      <arglist>(uint entry)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_diag_init</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a4a25c25939b98a0966b1248a52f06f27</anchor>
      <arglist>(const uint *table)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rtr_init</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a2550bb97884d308a9cccd5f4791e6de8</anchor>
      <arglist>(uint monitor)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const ushort</type>
      <name>pl340_data</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a2f80bce7a52da644c5f5292441b79378</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const uint</type>
      <name>dgf_init</name>
      <anchorfile>sark__hw_8c.html</anchorfile>
      <anchor>a0a8c4849ba3a8e317f00834e18287fe2</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sark_io.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/sark/</path>
    <filename>sark__io_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="no" imported="no">sark.h</includes>
    <class kind="struct">iobuf</class>
    <member kind="function" static="yes">
      <type>static sdp_msg_t *</type>
      <name>io_std_init</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a579f10a217f94b6bed355a7715259e91</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static iobuf_t *</type>
      <name>io_buf_init</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a682a055c3c0f1d1d09ee4959cfd3a958</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_io_buf_reset</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>afe00ab22d6ec00b7fc698270e7be8c28</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>io_put_char</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>ae3d6e0b4f6796acf8fb2dc494453598f</anchor>
      <arglist>(char *stream, uint c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>io_printf</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>abd43c3fa00f20bf106ace6fe769f941f</anchor>
      <arglist>(char *stream, char *f,...)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_str</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>add7e10cec4f2c79bba52b1d4891f4297</anchor>
      <arglist>(char *stream, char *s, int d)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_int</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a6a50f9c99ee59f80236df217710157dd</anchor>
      <arglist>(char *stream, int n, uint d, uint pad)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_uint</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a8385cc07ed4c2ce7ee1c0431e63f6605</anchor>
      <arglist>(char *stream, uint n, uint d, uint pad)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_zhex</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a4c8138ff4d19e18d5d4848abcaace604</anchor>
      <arglist>(char *stream, uint n, uint d)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_hex</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a8dc19e782ebae2a0782aaf243749debb</anchor>
      <arglist>(char *stream, uint n, uint d, uint pad)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_fixed</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a1ebfcca0a9a65d3cfe09e04d6cd83f1c</anchor>
      <arglist>(char *stream, uint n, uint d, uint a, uint pad, int neg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_sfixed</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a4432768bb8db8eeb4bc3da4e439eee9e</anchor>
      <arglist>(char *stream, int n, uint d, uint a, uint pad)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_ufixed</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a96509334b0e501da682d1fdaa867c073</anchor>
      <arglist>(char *stream, uint n, uint d, uint a, uint pad)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint</type>
      <name>sp_ptr</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>ab781a53bed42f857af65b5510521401c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint</type>
      <name>buf_ptr</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a4707a8a38e589d7d6e8a0b61b0dbd447</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static sdp_msg_t *</type>
      <name>io_msg</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a435bd64bfb05fb2dec6490bc423f3e97</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static iobuf_t *</type>
      <name>io_buf</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>af8cacbe1509101f30cdc65a181f2b33d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char</type>
      <name>hex</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a308d68c480d7b044ef7d036212e261d4</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_str</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>add7e10cec4f2c79bba52b1d4891f4297</anchor>
      <arglist>(char *stream, char *s, int d)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_int</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a6a50f9c99ee59f80236df217710157dd</anchor>
      <arglist>(char *stream, int n, uint d, uint pad)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_uint</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a8385cc07ed4c2ce7ee1c0431e63f6605</anchor>
      <arglist>(char *stream, uint n, uint d, uint pad)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_zhex</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a4c8138ff4d19e18d5d4848abcaace604</anchor>
      <arglist>(char *stream, uint n, uint d)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_hex</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a8dc19e782ebae2a0782aaf243749debb</anchor>
      <arglist>(char *stream, uint n, uint d, uint pad)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_fixed</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a1ebfcca0a9a65d3cfe09e04d6cd83f1c</anchor>
      <arglist>(char *stream, uint n, uint d, uint a, uint pad, int neg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_sfixed</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a4432768bb8db8eeb4bc3da4e439eee9e</anchor>
      <arglist>(char *stream, int n, uint d, uint a, uint pad)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>io_put_ufixed</name>
      <anchorfile>sark__io_8c.html</anchorfile>
      <anchor>a96509334b0e501da682d1fdaa867c073</anchor>
      <arglist>(char *stream, uint n, uint d, uint a, uint pad)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sark_isr.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/sark/</path>
    <filename>sark__isr_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="no" imported="no">sark.h</includes>
    <member kind="function">
      <type>void</type>
      <name>timer2_int</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>ab2cc514db37cda411e4f4861040148ec</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer2_int_han</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>ace30e4ba79be0d84354aa2e5761b9c7a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_fiq_han</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>af04f90a416df87565db777e5a3191910</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>txpkt_int_han</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>af5024ed78361476b301d17dd67a6766c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>user_null</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a7ad0c893c84c61c1c8991c125e5a6c38</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>user_irq</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a3f25722ef42ffb3bee4fdb2babdd46c8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>user_fiq</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a4cf51c928b6edf6903a0b6382c72150c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>user_queue</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>aa9bf7443e4222ee27209cd12a31733f1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>sdp_null</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a804034908df51e94f11737e28d158cfb</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>sdp_irq</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>ae079bf742bf561071d5c4dfcba1af44f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>sdp_fiq</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a52379f3c6bd3c3000dfc7ffc4a2231dd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>sdp_queue</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a9248b20069718b3a6eda3b63c594f268</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>rxpkt_null</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a5ddc207b2245645fe41a52178b62073a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>rxpkt_irq</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a14976428bc565d3d4bd113f6b45253c7</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>rxpkt_fiq</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>ac0ac429ebb4d83fcbed7ef4af82281fe</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>rxpkt_queue</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a02154b2f11a847376ab31f49cb48bab8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>timer_null</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>ac7fe68d09e79bd61fc753fdcb25cd84d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>timer_irq</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>ae2f12b3cc86866d3c129e245cd923092</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>timer_fiq</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>ac1ff9eab28848cf975cf592e08747552</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>timer_queue</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a156934d78406d194b713950d551e36a8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>sig_null</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a1d9820d6386a651fb2e29f6df027c8a8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>sig_irq</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a275233c95aa4a5ddcb11ff260348b16e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>sig_fiq</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>acfb0aa8b00426a225e083699e93a7d9a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>sig_queue</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a01b4263be28c65529fa87cf500bcb3a6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>const int_handler</type>
      <name>fiq_events</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a267cafbf516d3541b135164cbeffef69</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>const int_handler</type>
      <name>irq_events</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>abd32ccbdf19ca02e499bde715fe4d308</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>const int_handler</type>
      <name>queue_events</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>af58921dcc5779a845bb3f7b952d3d777</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>const int_handler</type>
      <name>null_events</name>
      <anchorfile>sark__isr_8c.html</anchorfile>
      <anchor>a9fa6d7772f994e693c56c13d50d41e1b</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sark_pkt.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/sark/</path>
    <filename>sark__pkt_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="no" imported="no">sark.h</includes>
    <member kind="function">
      <type>void</type>
      <name>txpkt_int_han</name>
      <anchorfile>sark__pkt_8c.html</anchorfile>
      <anchor>af5024ed78361476b301d17dd67a6766c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_register_pkt</name>
      <anchorfile>sark__pkt_8c.html</anchorfile>
      <anchor>a4175f7e6597dd3d01fff5b4afa13ea58</anchor>
      <arglist>(uint queue_size, vic_slot slot)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pkt_tx_kdc</name>
      <anchorfile>sark__pkt_8c.html</anchorfile>
      <anchor>aa5737fbb29e2f0f47e0375e4cba4162d</anchor>
      <arglist>(uint key, uint data, uint ctrl)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pkt_tx_kd</name>
      <anchorfile>sark__pkt_8c.html</anchorfile>
      <anchor>a5c544176d5d81edfa1485b5ae2e0d20f</anchor>
      <arglist>(uint key, uint data)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pkt_tx_k</name>
      <anchorfile>sark__pkt_8c.html</anchorfile>
      <anchor>a5950f6444f3c7afd3e6cdefec311a19d</anchor>
      <arglist>(uint key)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pkt_tx_kc</name>
      <anchorfile>sark__pkt_8c.html</anchorfile>
      <anchor>a0bace2238c993312c518553cdb237743</anchor>
      <arglist>(uint key, uint ctrl)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sark_timer.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/sark/</path>
    <filename>sark__timer_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="no" imported="no">sark.h</includes>
    <member kind="function">
      <type>void</type>
      <name>timer2_int_han</name>
      <anchorfile>sark__timer_8c.html</anchorfile>
      <anchor>ace30e4ba79be0d84354aa2e5761b9c7a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>event_register_timer</name>
      <anchorfile>sark__timer_8c.html</anchorfile>
      <anchor>a24995141a580df346fce85a38cbec1b8</anchor>
      <arglist>(vic_slot slot)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer_schedule</name>
      <anchorfile>sark__timer_8c.html</anchorfile>
      <anchor>a472cfc21a621f8e5153f1191342c5bc1</anchor>
      <arglist>(event_t *e, uint time)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>timer_schedule_proc</name>
      <anchorfile>sark__timer_8c.html</anchorfile>
      <anchor>aab4e6e587060d426aa55638664f1e3b6</anchor>
      <arglist>(event_proc proc, uint arg1, uint arg2, uint time)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer_cancel</name>
      <anchorfile>sark__timer_8c.html</anchorfile>
      <anchor>aacd1585a3489d9ee70bf5805214fe7f2</anchor>
      <arglist>(event_t *e, uint ID)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer_cancel_init</name>
      <anchorfile>sark__timer_8c.html</anchorfile>
      <anchor>a494d1ae311642ab308b728ecfe310342</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer2_int</name>
      <anchorfile>sark__timer_8c.html</anchorfile>
      <anchor>ab2cc514db37cda411e4f4861040148ec</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static event_t</type>
      <name>cancelled</name>
      <anchorfile>sark__timer_8c.html</anchorfile>
      <anchor>a4ae9ca81df7a48f924d091b45deceecc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sark.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>sark_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <member kind="function">
      <type>void</type>
      <name>c_main</name>
      <anchorfile>sark_8c.html</anchorfile>
      <anchor>aa14f4f1d4c84183b7bf7108bf930a23c</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>scamp-3.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>scamp-3_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <includes id="scamp_8h" name="scamp.h" local="yes" imported="no">scamp.h</includes>
    <includes id="spinn__net_8h" name="spinn_net.h" local="yes" imported="no">spinn_net.h</includes>
    <includes id="spinn__phy_8h" name="spinn_phy.h" local="yes" imported="no">spinn_phy.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>LIVENESS_FLASH_INTERVAL</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a4aec6857dc9e73663eb45ac70e3abd6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LIVENESS_FLASH_SPACING</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a79e559e53e998d7a7e6ab4592b946577</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOAD_FRAC_BITS</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a3dc188a8d48ee099c84515930e9299eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PWM_BITS</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>af1c503c50e6eb1b5e6cfbc63f2abb747</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>iptag_timer</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a43cf35e1883ddb6aeef2c36aa5d08be3</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>iptag_new</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>aced3331086dd2fceacb5e05be4d467c2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>transient_tag</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>acd73ad7fc94a8c3c44f01d63d196a2f5</anchor>
      <arglist>(uchar *ip, uchar *mac, uint port, uint timeout)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>queue_init</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ac19ac10c04e65b54e2c03aa726d1535a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_byte_set</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>af34678867589f8ad07326a39a7a2d0e0</anchor>
      <arglist>(uint a1, uint a2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_route_msg</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a12a3fae219a8e54a57ef9e239aa2c73c</anchor>
      <arglist>(uint arg1, uint srce_ip)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_queue_insert</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ab23849915c3d356f3faad678962df4e2</anchor>
      <arglist>(sdp_msg_t *msg, uint srce_ip)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pkt_tx</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a4be1e518c81bd6eaef9665aabc1d6f82</anchor>
      <arglist>(uint tcr, uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>timer1_init</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a99a9f0cb5f2e09948b33ae4cc58b400d</anchor>
      <arglist>(uint count)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eth_discard</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>aede4992481295221d4679098c144b2d4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>udp_pkt</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a915ddc5b73cac3a022447ebdf9f4a96e</anchor>
      <arglist>(uchar *rx_pkt, uint rx_len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eth_receive</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a9e6a70b18b433ae903057a4e151c4ab8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eth_send_msg</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a08de191dd5fe80911ad8f9c4bdd3cb2b</anchor>
      <arglist>(uint tag, sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>shm_ping</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ac01ad27e55d73265a8399eca0e1abe1e</anchor>
      <arglist>(uint dest)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>shm_send_msg</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a6f572adaa3e8746dc1e0e45dcc823641</anchor>
      <arglist>(uint dest, sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>swap_sdp_hdr</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a9ad8cf7bb1e7503bb00a6595e40d6386</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>return_msg</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a16d46d803b3eac8b95613bfe5ad11621</anchor>
      <arglist>(sdp_msg_t *msg, uint rc)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>assign_virt_cpu</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ac7f4aca3083672836a96abfe9b61e91b</anchor>
      <arglist>(uint phys_cpu)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remap_phys_cores</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ac86022e7df2a069555babe6bbbe8c7e2</anchor>
      <arglist>(uint phys_cores)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>ram_size</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a9e3746e63ad8ab631de478525df382ee</anchor>
      <arglist>(void *mem)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>get_board_info</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>aaba516d3db7c6462e763c6331b781a8a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sv_init</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ae8e652749ed73c8d4e67e17f076ab544</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sdram_init</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a6c3743105c81131817333ae7d2cf78a4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>random_init</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>acc9caf986ad394f055fe6f511f7109f6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>update_load</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a73c0c715a706c8657a49f1237d5870b3</anchor>
      <arglist>(uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_1hz</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a958cf122db02863d7eadabcec909b219</anchor>
      <arglist>(uint a1, uint a2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>soft_wdog</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a37ada33051cb5314c1f51bd0b6469941</anchor>
      <arglist>(uint max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>init_link_en</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>aaae405876cf186270249c81d1be6569a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>disable_unidirectional_links</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a5505bb7a8760b0b56f386c9745a87c3a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>netinit_start</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a253f7312791f5a8f625a02226ac97719</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>compute_st</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>aa681c1b75e6ae874f6fcb1003a4097a8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_100hz</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>aebbdf5b45c388d5074b3c861f19e5d0a</anchor>
      <arglist>(uint a1, uint a2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_1khz</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a865ac89f390bce1042687bdec54c67aa</anchor>
      <arglist>(uint a1, uint a2)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>pll_mult</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>acec65c78f768edd6bf217936688b1266</anchor>
      <arglist>(uint freq)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pll_init</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ad9e7fcfc46402ee91ee2d711c1135815</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eth_setup</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ab24a4e30929ac7cb61e9255a82eee104</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>jtag_init</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a1f1fefe5f455b8d8a85bccbdf7742780</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_config</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a735ec44ff9cf4f4f3c22ee4a163e8360</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delegate</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a0ab76cec7bf19e56ddbfd32f46943b77</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>chk_bl_del</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a811f865fd787dae69e77e8a14a704bbe</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c_main</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>aa14f4f1d4c84183b7bf7108bf930a23c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>num_cpus</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ac80496cd2e8e8cdeaacf7d6d0468c9c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>ping_cpu</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>adb0362e4900709d752c502075991d582</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>v2p_map</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a1c21072a8d78d1aab5893bcb3df847ba</anchor>
      <arglist>[MAX_CPUS]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>p2p_addr</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a4009dbfadbdf8d46b4579e9523017af2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>p2p_dims</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>abaf059496fdf2806160dfb3a7ed74de6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>p2p_root</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a5167468dafb6272e227768a1ac555572</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>p2p_up</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a3891cd79d8507e2f33dc37feca941fd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>ltpc_timer</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>aaa2cfc1437d2fb1728f771fba14937f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>link_en</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a1c6126739c93de2af3f25ef37b21872a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>srom_data_t</type>
      <name>srom</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a657415e0cd3fdaa7b3d23ac128803e98</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>pkt_queue_t</type>
      <name>tx_pkt_queue</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a601657bb2079605fac8e099adae3d7fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>watchdog</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>af33104e826e8822f68fe138a9aecfe60</anchor>
      <arglist>[20]</arglist>
    </member>
    <member kind="variable">
      <type>iptag_t</type>
      <name>tag_table</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ae1e977bb9b9c12c178303d20281bf865</anchor>
      <arglist>[TAG_TABLE_SIZE]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>tag_tto</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a5e83ef0d01511e03db4986730a2335cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>enum netinit_phase_e</type>
      <name>netinit_phase</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ae96fb6caf564cb7b62348bb25b846301</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>enum ethinit_phase_e</type>
      <name>ethinit_phase</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a94af9d8a357058b1829c61e089fa1e83</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uint</type>
      <name>ticks_since_last_p2pc_new</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a20cfb3b86cdbb54c13326ddcaa4308bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uint</type>
      <name>ticks_since_last_p2pc_dims</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a549a55706e1af8375f8d00dbe47bb7c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar *</type>
      <name>p2p_addr_table</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>aacfb094bef0ce893bc64247c11e207bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uchar</type>
      <name>load</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a436e9f1dd3c760793dc9949f822d5c55</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uint</type>
      <name>disp_load</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a0ebe75e3400852862a091528a3042259</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint</type>
      <name>rst_init</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a30993df3088be92a0df3616e9ec6f03a</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_addr_guess_x</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>aec31553feaeafafd07c2344d3fe10c93</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_addr_guess_y</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>abdfcb1341c4e50d13b8267f489f0d802</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_min_x</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a15fba20ebcbcda96e7d2d0a5e4f55a9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_max_x</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a1fe863b00af4e561a1b6858ff4581608</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_min_y</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a4b8501474dcfe44a125188ea3c0f6670</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_max_y</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ae010bc826c4042f17522a8269a6aa8a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_addr_guess_x</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>aec31553feaeafafd07c2344d3fe10c93</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_addr_guess_y</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>abdfcb1341c4e50d13b8267f489f0d802</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_min_x</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a15fba20ebcbcda96e7d2d0a5e4f55a9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_max_x</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a1fe863b00af4e561a1b6858ff4581608</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_min_y</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>a4b8501474dcfe44a125188ea3c0f6670</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_max_y</name>
      <anchorfile>scamp-3_8c.html</anchorfile>
      <anchor>ae010bc826c4042f17522a8269a6aa8a7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>scamp-app.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>scamp-app_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <includes id="scamp_8h" name="scamp.h" local="yes" imported="no">scamp.h</includes>
    <member kind="function">
      <type>void</type>
      <name>clock_ap</name>
      <anchorfile>scamp-app_8c.html</anchorfile>
      <anchor>ae531ffa5d5929f048fa44cde736e8ce0</anchor>
      <arglist>(uint phys_mask, uint enable)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset_ap</name>
      <anchorfile>scamp-app_8c.html</anchorfile>
      <anchor>a5ac3462acd70244f36047e09f0b5fd16</anchor>
      <arglist>(uint virt_mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>boot_ap</name>
      <anchorfile>scamp-app_8c.html</anchorfile>
      <anchor>a1401d54ad8ff4cb284105fac94a875a9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>signal_sark</name>
      <anchorfile>scamp-app_8c.html</anchorfile>
      <anchor>a3d28571f9c69f9aecc5c45ed50d397f2</anchor>
      <arglist>(uint cmd, uint data, uint virt_mask, uint phys_mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_cpu_info</name>
      <anchorfile>scamp-app_8c.html</anchorfile>
      <anchor>a79a8eaa55aa1c8c30468c78604593b37</anchor>
      <arglist>(uint virt_mask, uint state, uint app_id)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>find_lead</name>
      <anchorfile>scamp-app_8c.html</anchorfile>
      <anchor>ab98e20becc1f7c06f0bc525367f5f8ac</anchor>
      <arglist>(uint mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_start_app</name>
      <anchorfile>scamp-app_8c.html</anchorfile>
      <anchor>aeafcb2f9901c3ccd6c4c661557f420a9</anchor>
      <arglist>(uint aplx_addr, uint id_op_mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clean_app_id</name>
      <anchorfile>scamp-app_8c.html</anchorfile>
      <anchor>a04244170dae73d9ecfe6b62e5419f18a</anchor>
      <arglist>(uint app_id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_init_cores</name>
      <anchorfile>scamp-app_8c.html</anchorfile>
      <anchor>a2eaa10622f0ab19c1a5c5e7e81078ae1</anchor>
      <arglist>(uint virt_mask, uint phys_mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_stop_app</name>
      <anchorfile>scamp-app_8c.html</anchorfile>
      <anchor>a95c029b07a2d7e28907c3e81dfbc5a7f</anchor>
      <arglist>(uint app_id_and_virt_mask, uint app_mask_and_phys_mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_power_down</name>
      <anchorfile>scamp-app_8c.html</anchorfile>
      <anchor>a3b18bee105829ba4e55d3ff26da95e37</anchor>
      <arglist>(uint virt_mask, uint phys_mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>signal_app</name>
      <anchorfile>scamp-app_8c.html</anchorfile>
      <anchor>a60e5813d2920c3b5722b7ee45d3f2ca9</anchor>
      <arglist>(uint data)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>scamp-boot.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>scamp-boot_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <includes id="scamp_8h" name="scamp.h" local="yes" imported="no">scamp.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>BOOT_BUF</name>
      <anchorfile>scamp-boot_8c.html</anchorfile>
      <anchor>a8c14ded805657fc594d477e40358933f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FF_TARGET_MONITOR</name>
      <anchorfile>scamp-boot_8c.html</anchorfile>
      <anchor>a63d1434de905261ac191ccb6a75b65d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>boot_phases</name>
      <anchorfile>scamp-boot_8c.html</anchorfile>
      <anchor>a9a4ced941c6983f9c62d86bfa79161ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FF_START_PHASE_1</name>
      <anchorfile>scamp-boot_8c.html</anchorfile>
      <anchor>a9a4ced941c6983f9c62d86bfa79161aca28802a9c4cfe72353c735c0a8410555f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FF_BLOCK_START_PHASE_1</name>
      <anchorfile>scamp-boot_8c.html</anchorfile>
      <anchor>a9a4ced941c6983f9c62d86bfa79161aca1a00f7e65a92e6a425345d37794632bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FF_BLOCK_DATA_PHASE_1</name>
      <anchorfile>scamp-boot_8c.html</anchorfile>
      <anchor>a9a4ced941c6983f9c62d86bfa79161aca8fa65c86c4e86a7fc9d64622b7d65416</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FF_BLOCK_END_PHASE_1</name>
      <anchorfile>scamp-boot_8c.html</anchorfile>
      <anchor>a9a4ced941c6983f9c62d86bfa79161aca4741f7662330b15df028459a3f882c2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FF_CONTROL_PHASE_1</name>
      <anchorfile>scamp-boot_8c.html</anchorfile>
      <anchor>a9a4ced941c6983f9c62d86bfa79161acab91b73824e996c970c0850d783c89970</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>crc32</name>
      <anchorfile>scamp-boot_8c.html</anchorfile>
      <anchor>a0b2558d17ee0fb30a8c3c816c6d76ec3</anchor>
      <arglist>(const uchar *buf, uint len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_tx</name>
      <anchorfile>scamp-boot_8c.html</anchorfile>
      <anchor>a83fbf16ea2957916f6cda66902a72182</anchor>
      <arglist>(uint key, uint data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>boot_nn</name>
      <anchorfile>scamp-boot_8c.html</anchorfile>
      <anchor>aeafed943f53ac9e25a483de3745935ad</anchor>
      <arglist>(uint hw_ver)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const uint</type>
      <name>crc_table</name>
      <anchorfile>scamp-boot_8c.html</anchorfile>
      <anchor>a888676913964965514d431a010845e97</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>scamp-cmd.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>scamp-cmd_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <includes id="scamp_8h" name="scamp.h" local="yes" imported="no">scamp.h</includes>
    <includes id="spinn__net_8h" name="spinn_net.h" local="yes" imported="no">spinn_net.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>SCAMP_ID_STR</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>ae54868a2c4ffaf78e6f81738f5c69f1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>state_coalesce_mode</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a5e1efbbfd24bbb129fa0b848af91f6bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MODE_OR</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a5e1efbbfd24bbb129fa0b848af91f6bda4ec1b2da035462eb81e781e7db4e20ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MODE_AND</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a5e1efbbfd24bbb129fa0b848af91f6bda4eb4f2f58e38344407155db5707749cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MODE_SUM</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a5e1efbbfd24bbb129fa0b848af91f6bdad2897b63cbac7d6dc9454b98252533ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MODE_3</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a5e1efbbfd24bbb129fa0b848af91f6bdad94e70da03012c70efde66c9136b0981</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>send_reg_ctrl</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a5c65d633ff0a6204f34778e9652f7abb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>APP_RET</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a5c65d633ff0a6204f34778e9652f7abbabcf8baf265eb094e1f7a89cd75ce908a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>APP_STAT</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a5c65d633ff0a6204f34778e9652f7abba0d274f1614f420b81d2bb10fb2e6210a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>APP_SIG</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a5c65d633ff0a6204f34778e9652f7abba721904392091b8a0af752ee3ba60d142</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>APP_3</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a5c65d633ff0a6204f34778e9652f7abba80a5b0604a911accc4710cd613546c10</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>signal_type</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>af9ca40705a5c5f739ca699c48cb3b0f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_TYPE_MC</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>af9ca40705a5c5f739ca699c48cb3b0f7ae158900398cd5b27a5e84bfbfa9079d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_TYPE_P2P</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>af9ca40705a5c5f739ca699c48cb3b0f7abe0b93cdeb51b07d7fe2a7d5af5d989a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG_TYPE_NN</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>af9ca40705a5c5f739ca699c48cb3b0f7abc60f3faee10c165c80dd154abce74d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_nnp</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>ac96e650ca1f3079540da4d2c488e9aa7</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_ffd</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a56016d4a433f6f8e35a544b61d1fd0a8</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_iptag</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a8dfae827e59f9c7e5850aac375d3706d</anchor>
      <arglist>(sdp_msg_t *msg, uint srce_ip)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_ver</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a34c0a900efe27e94678596e47aa1f835</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_link_read</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a7a379fbbd359634d27f4e9778be86f85</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_link_write</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>ac97dc8b60f01b2cddb8da561edb738ba</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_as</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>acb839f5460263a2ab474dbe89f799928</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_ar</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a47f37ed8768002a2ec7e1be72cc126d4</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_rtr</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a31276d8c1cea992fd0461856c79a4902</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_info</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>ae80072aaf53c22d2a50704b8a022f264</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pkt_tx</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a4be1e518c81bd6eaef9665aabc1d6f82</anchor>
      <arglist>(uint tcr, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>return_msg</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a16d46d803b3eac8b95613bfe5ad11621</anchor>
      <arglist>(sdp_msg_t *msg, uint rc)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>p2p_send_reg</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>ae1532ca7d070c0943f25c97a29d49774</anchor>
      <arglist>(uint ctrl, uint addr, uint data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>proc_ret_msg</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>ac1ad9c67faf6043f7c019a6637786646</anchor>
      <arglist>(uint arg1, uint level)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>proc_gather</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a4e192d94fd80cac785175cfa1d6a4a22</anchor>
      <arglist>(uint level, uint mode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>proc_send</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a0a21843f6edee24726e210a5badb3558</anchor>
      <arglist>(uint data, uint mask)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>proc_process</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a45e0235be906f7ab0645230ee26333ba</anchor>
      <arglist>(uint data, uint srce)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_region</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>abd17fb13cb0d84ec27ca5d0d1ceff39c</anchor>
      <arglist>(uint data, uint srce)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_sig</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a4a6788278cb833a7e4c9e49984a70182</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_alloc</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>ae260a1109e7ddf05cd209abe4f5df26c</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_remap</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>aca400b8c3a3b9fa1a4a73800319141d1</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>scamp_debug</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>a5b830754727615d4f6bba4876f348c27</anchor>
      <arglist>(sdp_msg_t *msg, uint srce_ip)</arglist>
    </member>
    <member kind="variable">
      <type>level_t</type>
      <name>levels</name>
      <anchorfile>scamp-cmd_8c.html</anchorfile>
      <anchor>ad415696ea03a6a1d98e2247335840836</anchor>
      <arglist>[4]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>scamp-del.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>scamp-del_8c.html</filename>
    <includes id="spinnaker_8h" name="spinnaker.h" local="yes" imported="no">spinnaker.h</includes>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <includes id="scamp_8h" name="scamp.h" local="yes" imported="no">scamp.h</includes>
    <member kind="function">
      <type>void</type>
      <name>img_cp_exe</name>
      <anchorfile>scamp-del_8c.html</anchorfile>
      <anchor>a45bdf0600279ad2198d1a5db2a88203d</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>scamp-isr.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>scamp-isr_8c.html</filename>
    <includes id="spinnaker_8h" name="spinnaker.h" local="yes" imported="no">spinnaker.h</includes>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <includes id="scamp_8h" name="scamp.h" local="yes" imported="no">scamp.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>MC_SLOT</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a2774b920be5c242f4bae9cc73cd44dcf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_DIFF</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a1605a24e037309a2d85ed0e8dfcb6926</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>N_ITEMS</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a29a76405e22ea60f225ac3ab05c71e9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_region</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>ad74c8b7f5472e3c90d814834ccd418bf</anchor>
      <arglist>(uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_rcv_data</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a7192a69300207bcc88e26e7862cea759</anchor>
      <arglist>(uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_rcv_ctrl</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a6166d2d8879f01ec8f5e950ad7d9b54b</anchor>
      <arglist>(uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_rcv_pkt</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a38a39f4802a84cc8b44112fc770e523d</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>peek_ack_pkt</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>ab66629ed994a2e5c075ba72364d96533</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>poke_ack_pkt</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a0c82ef2660b8a0ae9594cce21c99d0f5</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_1khz</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a865ac89f390bce1042687bdec54c67aa</anchor>
      <arglist>(uint a1, uint a2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_100hz</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>aebbdf5b45c388d5074b3c861f19e5d0a</anchor>
      <arglist>(uint a1, uint a2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_1hz</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a958cf122db02863d7eadabcec909b219</anchor>
      <arglist>(uint a1, uint a2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_queue_insert</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>ab23849915c3d356f3faad678962df4e2</anchor>
      <arglist>(sdp_msg_t *msg, uint srce_ip)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pkt_tx_int</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>ac9c2bf6cfabdf2dd148fa3ceda458299</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eth_rx_int</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a1487b4007ac5e688cb200da53f978428</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pkt_mc_int</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a56314b1d2c006c0714d7b5e0c5179dbe</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pkt_nn_int</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a9376d7ff7d185dd5f7b12b11127609e1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pkt_p2p_int</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a9d582651a563c6363df4f9b8c8104685</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ms_timer_int</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a117ad44f7f618ca91d368f4dabe01b6d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ap_int</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>afb9acdcfeae794bdbb6e68bac529ea36</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer2_int_han</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>ace30e4ba79be0d84354aa2e5761b9c7a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>vic_setup</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>afcc02989a7ea9ec1887ffd977f3f3ffa</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>pkt_queue_t</type>
      <name>tx_pkt_queue</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a601657bb2079605fac8e099adae3d7fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>v2p_map</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a1c21072a8d78d1aab5893bcb3df847ba</anchor>
      <arglist>[MAX_CPUS]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>num_cpus</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>ac80496cd2e8e8cdeaacf7d6d0468c9c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint</type>
      <name>centi_ms</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a78a522438118d69a007af3b6e075c9e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>samples</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a538b8434d9d39893b5bcf4d70c403c8f</anchor>
      <arglist>[N_ITEMS]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>sum</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a930b83edda39ee8cfa78dd4574a50bc3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>last_ticks</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>aa057ab69e2a4242f35fcb898c45fa0a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint</type>
      <name>n_samples</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a9caebf6e1c8e5a380ecec14faa8cc24c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint</type>
      <name>sample_pos</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a191c7511b2a3210a258735cdad9238c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>last_beacon</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>ae5df0fadd48c0d82df01a81dc4400a2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint</type>
      <name>n_beacons_sent</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a6ee20526fd90716c9af350599ed326df</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint</type>
      <name>time_to_next_sync</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>a91b4e467c4c4cc1587943c62c73544bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint</type>
      <name>next_box</name>
      <anchorfile>scamp-isr_8c.html</anchorfile>
      <anchor>ac811df9df01bad3671723317adda9e51</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>scamp-nn.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>scamp-nn_8c.html</filename>
    <includes id="spinnaker_8h" name="spinnaker.h" local="yes" imported="no">spinnaker.h</includes>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <includes id="scamp_8h" name="scamp.h" local="yes" imported="no">scamp.h</includes>
    <class kind="struct">nn_desc_t</class>
    <member kind="define">
      <type>#define</type>
      <name>BC_TABLE_SIZE</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a653f2aebe1032dbb49309270ca6a8e29</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PART_ID</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ad435833c7c311994537ee9bdbf7e6d20</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2PB_STOP_BIT</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>acdffdb645573387a91fd86356a7c71e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>ff_states</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a9a9b5145f7df2f41b1723bd1f26cf8e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FF_ST_IDLE</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a9a9b5145f7df2f41b1723bd1f26cf8e2a28e107cc53260237771486b2a0c2a89a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FF_ST_EXBLK</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a9a9b5145f7df2f41b1723bd1f26cf8e2ab7587a5df17537816a35723f6229365c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FF_ST_INBLK</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a9a9b5145f7df2f41b1723bd1f26cf8e2ab69f6487e2c7f8557b5ff2f4978d2e87</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>sig0_operations</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a01bed5b553ed2f2b68bfb6c148d79d7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG0_FWD_RTRY</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a01bed5b553ed2f2b68bfb6c148d79d7da97bbd6e9e1dbdc6f401c105668c6d5bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG0_LEDS</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a01bed5b553ed2f2b68bfb6c148d79d7da8ba63c7222cb6d8db0e444417f8ac3f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG0_GTP</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a01bed5b553ed2f2b68bfb6c148d79d7dae5622ecc8b06d59df1c5ba7e59872c41</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG0_ID</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a01bed5b553ed2f2b68bfb6c148d79d7daeff36f16981800e089cbdd6e93b32420</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG0_LEVEL</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a01bed5b553ed2f2b68bfb6c148d79d7da0842f0f1d73536d29c9ec73ae5501c84</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG0_APP</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a01bed5b553ed2f2b68bfb6c148d79d7da78c557c2ae699f588009bc9ed3d8e32e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG0_TP</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a01bed5b553ed2f2b68bfb6c148d79d7daadff716b9b7631456a0fbb18b8d0e172</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG0_RST</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a01bed5b553ed2f2b68bfb6c148d79d7dafc5e21f0912b5a48ab10ac818911844b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG0_DOWN</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a01bed5b553ed2f2b68bfb6c148d79d7da124cc602aad977c8d29366fe7d073b73</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG0_UP</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a01bed5b553ed2f2b68bfb6c148d79d7daf0505fda74c194daddcf4e994323ff73</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>sig1_operations</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>af2a011bd4327fcf3099c5dfaf971fdce</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIG1_WRITE</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>af2a011bd4327fcf3099c5dfaf971fdcea832b815e3072397bcbd0142cb29fb0ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_init</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a8c5d005c87f72feb35160432e502583a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>compute_eth</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a8ebd349f2396adb3ad66f51b0eae7e2d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>compute_level</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>af88d0fa5fd70112d7b52a6f83a8335e2</anchor>
      <arglist>(uint p2p_addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>level_config</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a1b8e69ce037967ab8324b4542710c83f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>peek_ack_pkt</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ab66629ed994a2e5c075ba72364d96533</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>poke_ack_pkt</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a0c82ef2660b8a0ae9594cce21c99d0f5</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>link_read_word</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a6a6129cd30919afb3633b8abe8ac32f6</anchor>
      <arglist>(uint addr, uint link, uint *buf, uint timeout)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>link_write_word</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a7d7ed445d6a2770c002ec1a1847b42f3</anchor>
      <arglist>(uint addr, uint link, uint *buf, uint timeout)</arglist>
    </member>
    <member kind="function">
      <type>pkt_buf_t *</type>
      <name>pkt_buf_get</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a2c73fdf2c5fcf933d4dc69e08f4e0428</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pkt_buf_free</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a400cf3bc16c3bead82b7723ba0513273</anchor>
      <arglist>(pkt_buf_t *pkt)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_mark</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a980fd5ceb4a387c8536fd944295024fc</anchor>
      <arglist>(uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>nn_cmd_rtrc</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>abb59a3b1fe7a16ebc373ab04e541f64b</anchor>
      <arglist>(uint data)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>nn_cmd_sig0</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a3f3cf1c2cb135b4f2c6e13f813cd6398</anchor>
      <arglist>(uint data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>nn_cmd_mem</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>afcd96a50c3da441f4bdee91ba1aefbaf</anchor>
      <arglist>(uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_cmd_sig1</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a8938a548c63a40846b9695747393f3a5</anchor>
      <arglist>(uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>nn_cmd_p2pb</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a35d9621f7c11c490b4070c16ed4a3ed5</anchor>
      <arglist>(uint id, uint data, uint link)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>proc_pkt_bc</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ae934ebc67f5f6191da540e1c6c449952</anchor>
      <arglist>(uint i_pkt, uint count)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_rcv_pkt</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a38a39f4802a84cc8b44112fc770e523d</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>next_id</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a97edc6b1aeb8982af5d25bc1e15f4249</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ff_nn_send</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a5f820311b911d8204322b071c0f047bb</anchor>
      <arglist>(uint key, uint data, uint fwd_rty, uint add_id)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>nn_cmd_ffs</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a791e3702826f01e87750a687037669a3</anchor>
      <arglist>(uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>nn_cmd_ffcs</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a6615c699346157018c49048b1d04506d</anchor>
      <arglist>(uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>nn_cmd_fbs</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a242a12ae66ef45d25b6c8cd02b327178</anchor>
      <arglist>(uint id, uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>nn_cmd_fbd</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>aac0a16f7a3f145c594ca762b523064d4</anchor>
      <arglist>(uint id, uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>nn_cmd_fbe</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>aee7f198ef5c48f91a5c7f78a9c94d336</anchor>
      <arglist>(uint id, uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>nn_cmd_ffe</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a0d6f7115637e556a5b3999c999df8555</anchor>
      <arglist>(uint id, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>next_biff_id</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ae4c4fb6bfc693b0dca69d878d0c0b1f3</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>biff_nn_send</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a6f02b13bbeeafc5e9962a26981191f3f</anchor>
      <arglist>(uint data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_cmd_biff</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>abed40011561d3349ddcdd15655064d4b</anchor>
      <arglist>(uint x, uint y, uint data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_rcv_biff_pct</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a86608669fd8b1bdd4248894601e4844b</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2pc_addr_nn_send</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>aa6e7c77fb9c2c05dd54407173f76a225</anchor>
      <arglist>(uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2pc_new_nn_send</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a98cec5443a4838d13f7f5a49824f880b</anchor>
      <arglist>(uint x, uint y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2pc_dims_nn_send</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a105bc460b554ce9f2b1ea56da2f4e8d6</anchor>
      <arglist>(uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2pb_nn_send</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a2a2df43005bb304519c395011e69cb4b</anchor>
      <arglist>(uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>nn_rcv_p2pc_addr_pct</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a67246f9bc36bb50e33541d227849fa62</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>nn_rcv_p2pc_new_pct</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a2e3353aa3f23a0571ed0bb4e5a9e8b18</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>nn_rcv_p2pc_dims_pct</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a045bc9808d73503a7121f5318ab61ccf</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_rcv_p2pc_pct</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a892388f45c16849c408880a1b981b817</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="variable">
      <type>pkt_buf_t</type>
      <name>pkt_buf_table</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ab6f2f58fa9b324fb409869c446fc4f20</anchor>
      <arglist>[BC_TABLE_SIZE]</arglist>
    </member>
    <member kind="variable">
      <type>pkt_buf_t *</type>
      <name>pkt_buf_list</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a137bcefe7d77759ecc64dea3bd4dc6a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>pkt_buf_count</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>aa671510dc74fcee53a1647a289bd074c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>pkt_buf_max</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a4f309ba021fc825815b75aa3125c4ac5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>hop_table</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ac1464549cfd61d40b5f142ea5617dc4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>level_t</type>
      <name>levels</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ad415696ea03a6a1d98e2247335840836</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>core_app</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a042b07d52d6ffe2b0c5a5a38d0571c22</anchor>
      <arglist>[MAX_CPUS]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>app_mask</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ab506ab276e3c58a07e5df8d86dd551a0</anchor>
      <arglist>[256]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static nn_desc_t</type>
      <name>nn_desc</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a6368fbf04ba001611067bd2cb0c125d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>pkt_buf_t</type>
      <name>peek_pkt</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a47e61de95b69d91a595f3e04df129415</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>pkt_buf_t</type>
      <name>poke_pkt</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ac9b7395f9b9b002591958cac35033b1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>mon_del</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a6cee4e1b6ae56ab514bd02d7f432587d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>eth_map</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>afd644f0d0600e8358411c6562983582d</anchor>
      <arglist>[12][12]</arglist>
    </member>
    <member kind="variable">
      <type>const signed char</type>
      <name>lx</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a2f18f0dffc881ab4636b4bbb41282ce2</anchor>
      <arglist>[6]</arglist>
    </member>
    <member kind="variable">
      <type>const signed char</type>
      <name>ly</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ae809b3d65e10a0f3b7acc65086e938d1</anchor>
      <arglist>[6]</arglist>
    </member>
    <member kind="variable">
      <type>const signed char</type>
      <name>lx</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a2f18f0dffc881ab4636b4bbb41282ce2</anchor>
      <arglist>[6]</arglist>
    </member>
    <member kind="variable">
      <type>const signed char</type>
      <name>ly</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ae809b3d65e10a0f3b7acc65086e938d1</anchor>
      <arglist>[6]</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>next_id</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a97edc6b1aeb8982af5d25bc1e15f4249</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ff_nn_send</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a5f820311b911d8204322b071c0f047bb</anchor>
      <arglist>(uint key, uint data, uint fwd_rty, uint add_id)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>nn_cmd_ffs</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a791e3702826f01e87750a687037669a3</anchor>
      <arglist>(uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>nn_cmd_ffcs</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a6615c699346157018c49048b1d04506d</anchor>
      <arglist>(uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>nn_cmd_fbs</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a242a12ae66ef45d25b6c8cd02b327178</anchor>
      <arglist>(uint id, uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>nn_cmd_fbd</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>aac0a16f7a3f145c594ca762b523064d4</anchor>
      <arglist>(uint id, uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>nn_cmd_fbe</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>aee7f198ef5c48f91a5c7f78a9c94d336</anchor>
      <arglist>(uint id, uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>nn_cmd_ffe</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a0d6f7115637e556a5b3999c999df8555</anchor>
      <arglist>(uint id, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>next_biff_id</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ae4c4fb6bfc693b0dca69d878d0c0b1f3</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>biff_nn_send</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a6f02b13bbeeafc5e9962a26981191f3f</anchor>
      <arglist>(uint data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_cmd_biff</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>abed40011561d3349ddcdd15655064d4b</anchor>
      <arglist>(uint x, uint y, uint data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_rcv_biff_pct</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a86608669fd8b1bdd4248894601e4844b</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2pc_addr_nn_send</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>aa6e7c77fb9c2c05dd54407173f76a225</anchor>
      <arglist>(uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2pc_new_nn_send</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a98cec5443a4838d13f7f5a49824f880b</anchor>
      <arglist>(uint x, uint y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2pc_dims_nn_send</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a105bc460b554ce9f2b1ea56da2f4e8d6</anchor>
      <arglist>(uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2pb_nn_send</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a2a2df43005bb304519c395011e69cb4b</anchor>
      <arglist>(uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>nn_rcv_p2pc_addr_pct</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a67246f9bc36bb50e33541d227849fa62</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>nn_rcv_p2pc_new_pct</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a2e3353aa3f23a0571ed0bb4e5a9e8b18</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>nn_rcv_p2pc_dims_pct</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a045bc9808d73503a7121f5318ab61ccf</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_rcv_p2pc_pct</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a892388f45c16849c408880a1b981b817</anchor>
      <arglist>(uint link, uint data, uint key)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>scamp-p2p.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>scamp-p2p_8c.html</filename>
    <includes id="spinnaker_8h" name="spinnaker.h" local="yes" imported="no">spinnaker.h</includes>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <includes id="scamp_8h" name="scamp.h" local="yes" imported="no">scamp.h</includes>
    <includes id="spinn__net_8h" name="spinn_net.h" local="yes" imported="no">spinn_net.h</includes>
    <class kind="struct">rx_desc</class>
    <class kind="struct">tx_desc</class>
    <member kind="define">
      <type>#define</type>
      <name>P2P_NUM_STR</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>acfff5213baef8b08676be0d249fab050</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>p2p_tx_state</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a6bd144a5c32cc1705d3c25d1307d4a33</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>p2p_rx_state</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>adb224042dd314c8297f61dacdebd82d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>p2p_stats_indices</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DATA_TO</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27a6a3ac82aa3dd6bbb5b81209fcc626627</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ACK_TO</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27a18cef306f3c79d0422b5061fc5649395</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OPEN_TO</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27a043c78331cc359cc86d23a7e51d0c9d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CLOSE_TO</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27ab4a2d95ea4924b393fbdf0e803eb0d86</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_SENDS</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27a9f181b0439321762e36c8ffc653577e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_OPENS</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27a819242333315edd81c2f8a3dcef400ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_BUSY1</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27afd0605fbac15970df6c67699545fb312</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_BUSY2</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27a878d2fbc380a56b78aae1e65b380654a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_REJECTS</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27a5b8d278b6e2d548781babacbd3efe06c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TCOUNT</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27ae176560a37d81a11d636fe75f99e95a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OPEN_DUP</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27a24e27f6bac9529020c2048a8fcad28d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TX_FAIL</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27a1330fee713fa1ae374ab0d8cea4ed5c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OPEN_EVENT</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27a21f705bb8ded2603501fd1b977d55f29</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_OPEN_N</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac84fc5b3f2d601b5948dc99cdfcdab27a918e186b1f232f1175b70541b7349324</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>desc_init</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac766cedbd8ff6b4561a572b88d82bb01</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>p2p_send_data</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>accd3f939c94192df48bbc4c1feae1ae3</anchor>
      <arglist>(uint data, uint addr)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>p2p_send_ctl</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ab06c49e5cd4bd7a8f409190576ea5fa3</anchor>
      <arglist>(uint ctrl, uint addr, uint data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_data_ack</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>aebd3311e81dd6d5fe9d50bb2c0126979</anchor>
      <arglist>(uint data, uint srce)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_close_req</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a876c7b4656b01346b31f55fc1e4c6fbe</anchor>
      <arglist>(uint data, uint srce)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_ack_timeout</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a160f72f8b0d0ab826464003277dd7419</anchor>
      <arglist>(uint txd, uint a2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_open_timeout</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a307b56582035433be21dd622af8628ab</anchor>
      <arglist>(uint a, uint b)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>p2p_send_msg</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a2f24bacea48bb5718bb391c64527a2d4</anchor>
      <arglist>(uint addr, sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_data_timeout</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ad41f48e42f301629a61d037b9de748aa</anchor>
      <arglist>(uint rxd, uint a2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_open_ack</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a9a650c3982747a0ba4573b82d01f707e</anchor>
      <arglist>(uint data, uint srce)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_open_req</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a40de2803772ae17cf6e3b9b090e5e1ff</anchor>
      <arglist>(uint data, uint addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_close_timeout</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a0f823151a38c06a1e7fd9d2101a91d53</anchor>
      <arglist>(uint rxd, uint rid)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_close_ack</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ab6215975897e934eda3b321ba32cb413</anchor>
      <arglist>(uint data, uint srce)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_rcv_data</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a64f8d5e5f34a812dbd126370cc3920af</anchor>
      <arglist>(uint data, uint addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2p_rcv_ctrl</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ae98454f7128ac9671813c9debfecbf97</anchor>
      <arglist>(uint data, uint addr)</arglist>
    </member>
    <member kind="variable">
      <type>tx_desc_t</type>
      <name>tx_desc</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a365d0170fb5ea30c714900e7c0146fc8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>rx_desc_t</type>
      <name>rx_desc_table</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>abce74c3490a8e0b8bb6d62ac7701cb68</anchor>
      <arglist>[P2P_NUM_STR]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>p2p_stats</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a1cc7a3c00a414524358652ff8f959848</anchor>
      <arglist>[32]</arglist>
    </member>
    <member kind="variable">
      <type>const uint</type>
      <name>open_req_retry</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ac52b56a82e768144e7446cc48ac681d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint</type>
      <name>open_ack_retry</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a898bcae29b04d7a4356154fd45d6b007</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint</type>
      <name>data_ack_retry</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a643e4a3a3d956834a7e57b2ef17df014</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint</type>
      <name>close_req_retry</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a7a237335314e2fac316f62a0a5130f76</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint</type>
      <name>open_ack_time</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a40e79ffb4af4be688d0d5df973a2b860</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint</type>
      <name>data_ack_time</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>aa9890513801617fc8b4d0a8eb207bd33</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint</type>
      <name>data_time</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>aea2bd5fa9dfd3614487ee147179bedfb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint</type>
      <name>close_ack_time</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>ae93b432ce8ba390d291de74b532c004c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>scamp.h</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>scamp_8h.html</filename>
    <class kind="struct">iptag_t</class>
    <class kind="struct">pkt_queue_t</class>
    <class kind="struct">pkt_buf_t</class>
    <class kind="struct">level_t</class>
    <member kind="define">
      <type>#define</type>
      <name>MAX_CPUS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a87cbc7cff225b4ad63d67d47c21f933f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MONITOR_CPU</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ac82150bc580d3e3ef5647be85d079304</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCAMP_MC_ROUTING_KEY</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6831c7253fdfdc02ab529400100e30ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCAMP_MC_ROUTING_MASK</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a4c05b412fb7bf062196cab276f382440</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCAMP_MC_TIME_SYNC_KEY</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a438007d0a814bab938f76d3e185500cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCAMP_MC_SIGNAL_KEY</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ac01ccdb8c08824c4c840e85fabb7563e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BLOCK_COUNT</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8b7771bbb0e029e9200b3a7ec9816204</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WORD_COUNT</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a1a73994c0f34d7160878e5307f221f60</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BYTE_COUNT</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af0427aee3a6302a998cd907c06e73470</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BOOT_IMAGE_SIZE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a4787ac9327243fabb5ec15ea2f80d9a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NODE_DLY_NS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>acb9f8fcbb74c4d411b39ed05ed24ae0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BRD_DLY_NS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aacb3ab84baf39c7749b5daac2e872406</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOP_DLY_US</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a9480c887a9fb09ed8e47051546b5f4ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NN_HOP_MASK</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>afb0bb87f6f4f65e5600ed5c1fe3c6ad9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HOP_TABLE_SIZE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a81ba10517e5fc5404269481c788d3383</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PKT_QUEUE_SIZE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>abbb67719a1d8919da0a8e73c9332d6f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>alloc_cmd_e</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a732aae634aa612c75b1f208141d3643d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ALLOC_SDRAM</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a732aae634aa612c75b1f208141d3643da25555a7bb62fe1955ecdd54cad62dee5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FREE_SDRAM</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a732aae634aa612c75b1f208141d3643da3be6d0a494aee4cf5b1ddf923b118c7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FREE_SDRAM_ID</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a732aae634aa612c75b1f208141d3643dab245e177408e0db376fc840be543214d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ALLOC_RTR</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a732aae634aa612c75b1f208141d3643daf9f96ed48994035ea064b8ad3e73404e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FREE_RTR</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a732aae634aa612c75b1f208141d3643dae5d28e26943e4f750391355482fd53cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FREE_RTR_ID</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a732aae634aa612c75b1f208141d3643daccd0af6fd9f2b9d63d42b0129393bf77</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SDRAM_SPACE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a732aae634aa612c75b1f208141d3643da8e3eef34a0e9d2f840fba98eca463347</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HEAP_TAG_PTR</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a732aae634aa612c75b1f208141d3643dab10e6ad957a24fe9204d4821a4eca351</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ALLOC_MAX</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a732aae634aa612c75b1f208141d3643da345b0a21856b784a02f10003bb1804be</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>scamp_sdp_flags</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a2f9a6b0247d45bd7f35f733e393e345a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SDPF_REPLY</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a2f9a6b0247d45bd7f35f733e393e345aa0b0ac1c3b32d12e6c2ac6318cc0a589f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SDPF_SUM</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a2f9a6b0247d45bd7f35f733e393e345aa16cb8d1682abe6fd12667ac23f0a7fa4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SDPF_NR</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a2f9a6b0247d45bd7f35f733e393e345aa35d6cd444d7203d7a0c67a19955b626a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>scamp_failure_codes</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_RESET</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a74b346abde17c9e3e083d34395ae6975</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_ROMX</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a14260427d12d40530350ae1af904b562</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_ITCM0</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a2bbcba0d1acef5428f88234102003985</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_ITCM1</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a7083ecc5cab90813b1e7d9694b8ebc4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_DTCM</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41aeadb93e54a76f2405d995967c915c333</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_TIMER</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a2002a298b36b40d7c7c67d0d8c0cabb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_VIC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a48afb6026547ba5c2be5c0ed4a9d1185</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_CC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41ac538fe04aed058d7db9307e738d51244</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_DMA</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a886b5416e93374b2e9a5e9909454c8c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_MP</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a6d10881a31bd52e471996670625d7157</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_LATE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41aba66c8c509e20c0072ae7b8583097664</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_MANUF</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a97672bbbeca39d7b52f1cf3f795e0320</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_SLEEP</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41abfc3466705cf05a81c59800715c1067e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_TLM</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a8b05880b4a7d2364fda1f94b5a4bc306</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_VEC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a025546e9e3ff8e0bd38228a36b5362f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>scamp_nn_opcodes</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_SIG0</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a913b5045a98bbb1582c42031a6f1dee9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_RTRC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a7c92ed6ef695a427b989c54b90b0dd5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_LTPC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a286ce33fb53397844accd70acae2cb62</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_SP_3</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a1b48805a7fc6a64756020997b9884f41</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_SIG1</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a4e4089a9bb6771677547b6380aabd4d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_P2PC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653ab941e870dc2f4438b23f68fbd1b0f8c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_FFS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a90e21c4c02f6671f786873470b15a26c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_FFCS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653aa46fbd08af1a6c9655435fb5f616e53f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_P2PB</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a55ed9a821c6f2525384ecb81fd46a8dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_SP_9</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653af2d947cd2c885c3f3f8f939adae77822</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_SP_10</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a9a41c2169b975a21345a9c26cf767b71</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_BIFF</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653abf703c041bec502b309261b82612ca46</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_FBS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a29b9298b2a5bb037586aba3438cbf80d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>scamp_nn_p2p_config_subcommands</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ad23fbc028718d6180d16767b141c6106</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2PC_ADDR</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ad23fbc028718d6180d16767b141c6106a9db8898ddc79ae39626b35d0f9600b1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2PC_NEW</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ad23fbc028718d6180d16767b141c6106af502384bd469c9549d3331f283ac6216</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2PC_DIMS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ad23fbc028718d6180d16767b141c6106a2c8ea4a3d04d84c75587a7e503e025cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>netinit_phase_e</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_P2P_ADDR</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699af6230f149bbc87b9ceebc16e618b7ce2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_P2P_DIMS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699aed7f2a4bab5c345a2a953b605f1b6a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_BIFF</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699aae9306bdec4caca4457d5f67a25ed3f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_P2P_TABLE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699a36ba9d1db713956758bc0736ba5daab8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_SET_ETHERNET_ADDR</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699a3bd0436dedc109f7ea6186a945593557</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_DEL</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699a03bed6b491c267c055f4e5c129bcbc5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_DONE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699abf37a9ca578bf92fa74577158f58ce73</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>ethinit_phase_e</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8f6a34d1ef48e00fa476e3ddc76ff925</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ETHINIT_PHASE_WAIT_1</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8f6a34d1ef48e00fa476e3ddc76ff925a402cca317853fe37a01d3c8c79a2bc11</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ETHINIT_PHASE_WAIT_2</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8f6a34d1ef48e00fa476e3ddc76ff925a302f426f95a4c067943a7e3b51875627</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ETHINIT_PHASE_DONE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8f6a34d1ef48e00fa476e3ddc76ff925ab3e60b4020978595af659ac7a118f9e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_srom</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a4c0ebee708bfd4951c0359ff6b869bb2</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>vic_setup</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>afcc02989a7ea9ec1887ffd977f3f3ffa</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>img_cp_exe</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a45bdf0600279ad2198d1a5db2a88203d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_start_app</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aeafcb2f9901c3ccd6c4c661557f420a9</anchor>
      <arglist>(uint aplx_addr, uint id_op_mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>signal_app</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a60e5813d2920c3b5722b7ee45d3f2ca9</anchor>
      <arglist>(uint data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>boot_ap</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a1401d54ad8ff4cb284105fac94a875a9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clock_ap</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aabc5f6f31332bb4cc073ed42a3563af1</anchor>
      <arglist>(uint virt_mask, uint enable)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset_ap</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a5ac3462acd70244f36047e09f0b5fd16</anchor>
      <arglist>(uint virt_mask)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>scamp_debug</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a5b830754727615d4f6bba4876f348c27</anchor>
      <arglist>(sdp_msg_t *msg, uint srce_ip)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>boot_nn</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aeafed943f53ac9e25a483de3745935ad</anchor>
      <arglist>(uint hw_ver)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_CPUS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a87cbc7cff225b4ad63d67d47c21f933f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MONITOR_CPU</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ac82150bc580d3e3ef5647be85d079304</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCAMP_MC_ROUTING_KEY</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6831c7253fdfdc02ab529400100e30ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCAMP_MC_ROUTING_MASK</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a4c05b412fb7bf062196cab276f382440</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCAMP_MC_TIME_SYNC_KEY</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a438007d0a814bab938f76d3e185500cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCAMP_MC_SIGNAL_KEY</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ac01ccdb8c08824c4c840e85fabb7563e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BLOCK_COUNT</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8b7771bbb0e029e9200b3a7ec9816204</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WORD_COUNT</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a1a73994c0f34d7160878e5307f221f60</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BYTE_COUNT</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af0427aee3a6302a998cd907c06e73470</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BOOT_IMAGE_SIZE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a4787ac9327243fabb5ec15ea2f80d9a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TAG_FIXED_SIZE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a986c98b61105e66e8b5b29fbcf79fd08</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TAG_POOL_SIZE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a9a6f7e5bb3da93fb42234eaa335a98b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FIRST_POOL_TAG</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ab8dc224792aa20bbd35eb65f7dac642c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LAST_POOL_TAG</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a1db952a59c44ec1c838d81e1a261a209</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TAG_TABLE_SIZE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aa28addc3189246782e4810ed1c46e2fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>scamp_iptag_commands</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a80504fd082efa3bfa8c5604e2d9e90ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IPTAG_NEW</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a80504fd082efa3bfa8c5604e2d9e90eea6cfb2b8a576c144d16c5809c6eedd9fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IPTAG_SET</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a80504fd082efa3bfa8c5604e2d9e90eea5338268458f5e7137ea380873691e61d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IPTAG_GET</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a80504fd082efa3bfa8c5604e2d9e90eeae361b2224243aae91c7f86be968d8607</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IPTAG_CLR</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a80504fd082efa3bfa8c5604e2d9e90eeae587107b5ac79e19abe2b99c341a9331</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IPTAG_TTO</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a80504fd082efa3bfa8c5604e2d9e90eea1dfa18932eaf9be3158408abe1774f2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IPTAG_MAX</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a80504fd082efa3bfa8c5604e2d9e90eea904da2250661e83e94892917893ae131</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>scamp_iptag_flags</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a9818991fe1d31ab0e2fcd91684889aaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IPFLAG_VALID</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a9818991fe1d31ab0e2fcd91684889aafa1dab3a82bb8fa15db8b2ee6e291153e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IPFLAG_TRANS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a9818991fe1d31ab0e2fcd91684889aafa8847e1b1432f333109341978a212a24a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IPFLAG_ARP</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a9818991fe1d31ab0e2fcd91684889aafadfb4ad0b07654650443c0664b39eddb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IPFLAG_USE_SENDER</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a9818991fe1d31ab0e2fcd91684889aafa67d712080f466ffd9ed78e6ee4a28480</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IPFLAG_REV</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a9818991fe1d31ab0e2fcd91684889aafa21fe401a4ab62a120eddd46a58e678e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IPFLAG_STRIP</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a9818991fe1d31ab0e2fcd91684889aafa8a2626bbfc15dc375c5f4d6844740f95</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>scamp_sdp_flags</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a2f9a6b0247d45bd7f35f733e393e345a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SDPF_REPLY</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a2f9a6b0247d45bd7f35f733e393e345aa0b0ac1c3b32d12e6c2ac6318cc0a589f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SDPF_SUM</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a2f9a6b0247d45bd7f35f733e393e345aa16cb8d1682abe6fd12667ac23f0a7fa4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SDPF_NR</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a2f9a6b0247d45bd7f35f733e393e345aa35d6cd444d7203d7a0c67a19955b626a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>scamp_failure_codes</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_RESET</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a74b346abde17c9e3e083d34395ae6975</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_ROMX</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a14260427d12d40530350ae1af904b562</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_ITCM0</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a2bbcba0d1acef5428f88234102003985</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_ITCM1</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a7083ecc5cab90813b1e7d9694b8ebc4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_DTCM</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41aeadb93e54a76f2405d995967c915c333</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_TIMER</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a2002a298b36b40d7c7c67d0d8c0cabb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_VIC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a48afb6026547ba5c2be5c0ed4a9d1185</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_CC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41ac538fe04aed058d7db9307e738d51244</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_DMA</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a886b5416e93374b2e9a5e9909454c8c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_MP</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a6d10881a31bd52e471996670625d7157</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_LATE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41aba66c8c509e20c0072ae7b8583097664</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_MANUF</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a97672bbbeca39d7b52f1cf3f795e0320</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_SLEEP</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41abfc3466705cf05a81c59800715c1067e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_TLM</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a8b05880b4a7d2364fda1f94b5a4bc306</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FAIL_VEC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af76f97ec540d3a34728d2860c3db4d41a025546e9e3ff8e0bd38228a36b5362f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>scamp_nn_opcodes</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_SIG0</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a913b5045a98bbb1582c42031a6f1dee9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_RTRC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a7c92ed6ef695a427b989c54b90b0dd5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_LTPC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a286ce33fb53397844accd70acae2cb62</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_SP_3</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a1b48805a7fc6a64756020997b9884f41</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_SIG1</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a4e4089a9bb6771677547b6380aabd4d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_P2PC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653ab941e870dc2f4438b23f68fbd1b0f8c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_FFS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a90e21c4c02f6671f786873470b15a26c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_FFCS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653aa46fbd08af1a6c9655435fb5f616e53f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_P2PB</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a55ed9a821c6f2525384ecb81fd46a8dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_SP_9</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653af2d947cd2c885c3f3f8f939adae77822</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_SP_10</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a9a41c2169b975a21345a9c26cf767b71</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_BIFF</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653abf703c041bec502b309261b82612ca46</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NN_CMD_FBS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af04d3f5288e53ab1b6a585bffc694653a29b9298b2a5bb037586aba3438cbf80d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>scamp_nn_p2p_config_subcommands</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ad23fbc028718d6180d16767b141c6106</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2PC_ADDR</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ad23fbc028718d6180d16767b141c6106a9db8898ddc79ae39626b35d0f9600b1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2PC_NEW</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ad23fbc028718d6180d16767b141c6106af502384bd469c9549d3331f283ac6216</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2PC_DIMS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ad23fbc028718d6180d16767b141c6106a2c8ea4a3d04d84c75587a7e503e025cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_DEF_SQL</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a7787a01a3fb8d77785b8fbe9e0de81ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NO_IDEA</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a13cc1cb18ee3d570f0b97fa88310302d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2P_ADDR_TABLE_BYTES</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a74e29dd0c6352367639f0223aebca993</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>P2PB_OFFSET_USEC</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ae3369a03e4a48aa31c3e68d3d8854ca4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>scamp_p2p_type_codes</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a674d38d5c346910def25520a9d5505c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_DATA</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a674d38d5c346910def25520a9d5505c0a683c73ba28efb7d939c7c9845b460ae2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_CTRL</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a674d38d5c346910def25520a9d5505c0a4c6f1174d48c1b6434fc3daeda52d7b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_LEVEL</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a674d38d5c346910def25520a9d5505c0a64c4c01041f6edb1e9c1a6d1149ac347</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>scamp_p2p_control_message_codes</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a5f7bf8060bf3f077519a11b991abdc42</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_OPEN_REQ</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a5f7bf8060bf3f077519a11b991abdc42a31e5721aa98ef81b8d485ab2c4d8da53</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_OPEN_ACK</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a5f7bf8060bf3f077519a11b991abdc42a8ea263960f5e35b9eb4fcca18300cac1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_DATA_ACK</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a5f7bf8060bf3f077519a11b991abdc42ae9023f4ed96b069da70ab704d1706740</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_CLOSE_REQ</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a5f7bf8060bf3f077519a11b991abdc42a87d2b693106e050b7741568ef8f02d32</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>P2P_CLOSE_ACK</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a5f7bf8060bf3f077519a11b991abdc42a1cbf9729f6b5a985f723881a997aebfc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NODE_DLY_NS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>acb9f8fcbb74c4d411b39ed05ed24ae0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BRD_DLY_NS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aacb3ab84baf39c7749b5daac2e872406</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOP_DLY_US</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a9480c887a9fb09ed8e47051546b5f4ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NN_HOP_MASK</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>afb0bb87f6f4f65e5600ed5c1fe3c6ad9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HOP_TABLE_SIZE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a81ba10517e5fc5404269481c788d3383</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>netinit_phase_e</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_P2P_ADDR</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699af6230f149bbc87b9ceebc16e618b7ce2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_P2P_DIMS</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699aed7f2a4bab5c345a2a953b605f1b6a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_BIFF</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699aae9306bdec4caca4457d5f67a25ed3f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_P2P_TABLE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699a36ba9d1db713956758bc0736ba5daab8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_SET_ETHERNET_ADDR</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699a3bd0436dedc109f7ea6186a945593557</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_DEL</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699a03bed6b491c267c055f4e5c129bcbc5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>NETINIT_PHASE_DONE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6651118d8fd7f3196f7e9845afe27699abf37a9ca578bf92fa74577158f58ce73</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>ethinit_phase_e</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8f6a34d1ef48e00fa476e3ddc76ff925</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ETHINIT_PHASE_WAIT_1</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8f6a34d1ef48e00fa476e3ddc76ff925a402cca317853fe37a01d3c8c79a2bc11</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ETHINIT_PHASE_WAIT_2</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8f6a34d1ef48e00fa476e3ddc76ff925a302f426f95a4c067943a7e3b51875627</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ETHINIT_PHASE_DONE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8f6a34d1ef48e00fa476e3ddc76ff925ab3e60b4020978595af659ac7a118f9e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PKT_QUEUE_SIZE</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>abbb67719a1d8919da0a8e73c9332d6f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>srom_data_t</type>
      <name>srom</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a657415e0cd3fdaa7b3d23ac128803e98</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>link_en</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a1c6126739c93de2af3f25ef37b21872a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>num_cpus</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ac80496cd2e8e8cdeaacf7d6d0468c9c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>iptag_t</type>
      <name>tag_table</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a290212ce4c02692812b2f35ab7513bf1</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>tag_tto</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a5e83ef0d01511e03db4986730a2335cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>v2p_map</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a1c21072a8d78d1aab5893bcb3df847ba</anchor>
      <arglist>[MAX_CPUS]</arglist>
    </member>
    <member kind="variable">
      <type>enum netinit_phase_e</type>
      <name>netinit_phase</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ae96fb6caf564cb7b62348bb25b846301</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>enum ethinit_phase_e</type>
      <name>ethinit_phase</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a94af9d8a357058b1829c61e089fa1e83</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>pkt_tx</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a4be1e518c81bd6eaef9665aabc1d6f82</anchor>
      <arglist>(uint tcr, uint data, uint key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_byte_set</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af34678867589f8ad07326a39a7a2d0e0</anchor>
      <arglist>(uint a1, uint a2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_queue_insert</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ab23849915c3d356f3faad678962df4e2</anchor>
      <arglist>(sdp_msg_t *msg, uint srce_ip)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>iptag_new</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aced3331086dd2fceacb5e05be4d467c2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>assign_virt_cpu</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ac7f4aca3083672836a96abfe9b61e91b</anchor>
      <arglist>(uint phys_cpu)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remap_phys_cores</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ac86022e7df2a069555babe6bbbe8c7e2</anchor>
      <arglist>(uint phys_cores)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delegate</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a0ab76cec7bf19e56ddbfd32f46943b77</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>mon_del</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6cee4e1b6ae56ab514bd02d7f432587d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>hop_table</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ac1464549cfd61d40b5f142ea5617dc4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>core_app</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a042b07d52d6ffe2b0c5a5a38d0571c22</anchor>
      <arglist>[MAX_CPUS]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>app_mask</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ab506ab276e3c58a07e5df8d86dd551a0</anchor>
      <arglist>[256]</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>compute_eth</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8ebd349f2396adb3ad66f51b0eae7e2d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>compute_level</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>af88d0fa5fd70112d7b52a6f83a8335e2</anchor>
      <arglist>(uint p2p_addr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>level_config</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a1b8e69ce037967ab8324b4542710c83f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ff_nn_send</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>adda431127b74cfcee87b060c5bbd692c</anchor>
      <arglist>(uint key, uint data, uint fwd_rty, uint log)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>biff_nn_send</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6f02b13bbeeafc5e9962a26981191f3f</anchor>
      <arglist>(uint data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_cmd_biff</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>abed40011561d3349ddcdd15655064d4b</anchor>
      <arglist>(uint x, uint y, uint data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_mark</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a980fd5ceb4a387c8536fd944295024fc</anchor>
      <arglist>(uint key)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>link_read_word</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6a6129cd30919afb3633b8abe8ac32f6</anchor>
      <arglist>(uint addr, uint link, uint *buf, uint timeout)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>link_write_word</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a7d7ed445d6a2770c002ec1a1847b42f3</anchor>
      <arglist>(uint addr, uint link, uint *buf, uint timeout)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>next_id</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a97edc6b1aeb8982af5d25bc1e15f4249</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>nn_init</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8c5d005c87f72feb35160432e502583a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_ffe</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a707dcd5bda29bd31dc2b4b5353c62cd7</anchor>
      <arglist>(uint aplx_addr, uint cpu_mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2pc_addr_nn_send</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aa6e7c77fb9c2c05dd54407173f76a225</anchor>
      <arglist>(uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2pc_dims_nn_send</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a105bc460b554ce9f2b1ea56da2f4e8d6</anchor>
      <arglist>(uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>p2pb_nn_send</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a2a2df43005bb304519c395011e69cb4b</anchor>
      <arglist>(uint arg1, uint arg2)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_srom</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a4c0ebee708bfd4951c0359ff6b869bb2</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>vic_setup</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>afcc02989a7ea9ec1887ffd977f3f3ffa</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>img_cp_exe</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a45bdf0600279ad2198d1a5db2a88203d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>proc_start_app</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aeafcb2f9901c3ccd6c4c661557f420a9</anchor>
      <arglist>(uint aplx_addr, uint id_op_mask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>signal_app</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a60e5813d2920c3b5722b7ee45d3f2ca9</anchor>
      <arglist>(uint data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>boot_ap</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a1401d54ad8ff4cb284105fac94a875a9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clock_ap</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aabc5f6f31332bb4cc073ed42a3563af1</anchor>
      <arglist>(uint virt_mask, uint enable)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset_ap</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a5ac3462acd70244f36047e09f0b5fd16</anchor>
      <arglist>(uint virt_mask)</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>p2p_addr</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a4009dbfadbdf8d46b4579e9523017af2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>p2p_dims</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>abaf059496fdf2806160dfb3a7ed74de6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>p2p_root</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a5167468dafb6272e227768a1ac555572</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>p2p_up</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a3891cd79d8507e2f33dc37feca941fd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uint</type>
      <name>ticks_since_last_p2pc_new</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a20cfb3b86cdbb54c13326ddcaa4308bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uint</type>
      <name>ticks_since_last_p2pc_dims</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a549a55706e1af8375f8d00dbe47bb7c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_addr_guess_x</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aec31553feaeafafd07c2344d3fe10c93</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_addr_guess_y</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>abdfcb1341c4e50d13b8267f489f0d802</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_min_x</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a15fba20ebcbcda96e7d2d0a5e4f55a9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_max_x</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a1fe863b00af4e561a1b6858ff4581608</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_min_y</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a4b8501474dcfe44a125188ea3c0f6670</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile int</type>
      <name>p2p_max_y</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ae010bc826c4042f17522a8269a6aa8a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar *</type>
      <name>p2p_addr_table</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aacfb094bef0ce893bc64247c11e207bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>p2p_send_msg</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a2f24bacea48bb5718bb391c64527a2d4</anchor>
      <arglist>(uint addr, sdp_msg_t *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>desc_init</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ac766cedbd8ff6b4561a572b88d82bb01</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>scamp_debug</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a5b830754727615d4f6bba4876f348c27</anchor>
      <arglist>(sdp_msg_t *msg, uint srce_ip)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>boot_nn</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aeafed943f53ac9e25a483de3745935ad</anchor>
      <arglist>(uint hw_ver)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>spinn_net.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>spinn__net_8c.html</filename>
    <includes id="spinnaker_8h" name="spinnaker.h" local="yes" imported="no">spinnaker.h</includes>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <includes id="scamp_8h" name="scamp.h" local="yes" imported="no">scamp.h</includes>
    <includes id="spinn__net_8h" name="spinn_net.h" local="yes" imported="no">spinn_net.h</includes>
    <class kind="struct">arp_entry</class>
    <member kind="define">
      <type>#define</type>
      <name>MAX_ARP_ENTRIES</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a5742bcae4f323492c49bca65747ffa64</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>eth_discard</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a3b5b9ad96026c63c3f38eabc6cb56232</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eth_init</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>ab3e1723eb1dc1c5c927c6ca06463064c</anchor>
      <arglist>(uchar *mac)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>ipsum</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>aed20e55950dc2a5b79f8cd8fc1eca42b</anchor>
      <arglist>(uchar *d, uint len, uint sum)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>copy_mac</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a527085d546d3fd3877fb49fc0e3ad8ed</anchor>
      <arglist>(const uchar *f, uchar *t)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>copy_ip</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a930b67a993941fb4d6d5554d715b4a0b</anchor>
      <arglist>(const uchar *f, uchar *t)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmp_ip</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>aa31c42aac8f8fb6fe081c3b63acf37a7</anchor>
      <arglist>(const uchar *a, const uchar *b)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>copy_ip_hdr</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a85febc90e389ed3345cde8c1d3544aed</anchor>
      <arglist>(uchar *dest, uint prot, ip_hdr_t *ip, uint len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eth_transmit</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a2a0803ce8298c4b35e566f9d0ec7ce92</anchor>
      <arglist>(uchar *buf, uint len, uint type, const uchar *dest)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eth_transmit2</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>af730bd3b9bb23eaad8975626e6dd48a3</anchor>
      <arglist>(uchar *hdr, uchar *buf, uint hdr_len, uint buf_len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>send_arp_pkt</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a1d830a83487df866b7651f8a4392ea7f</anchor>
      <arglist>(uchar *buf, const uchar *dest, const uchar *tha, const uchar *tpa, uint type)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>arp_lookup</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a334e3df9be68b50d1d05dbf146144983</anchor>
      <arglist>(iptag_t *iptag)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>arp_pkt</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a3b8c480d1e4699fdb7d5de0f2b43cac8</anchor>
      <arglist>(uchar *rx_pkt, uint rx_len, uint tag_table_size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>arp_add</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>ac8f7e567b716b0e88a9e52f2364de2ce</anchor>
      <arglist>(uchar *mac, uchar *ip)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>icmp_pkt</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a6ac87dfdea22ccb270541502db9bbbc3</anchor>
      <arglist>(uchar *rx_pkt, uint rx_len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>copy_udp</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>aa9c6b4312768b7998223f802c861ff37</anchor>
      <arglist>(uchar *buf, uint len, uint dest, uint srce)</arglist>
    </member>
    <member kind="variable">
      <type>srom_data_t</type>
      <name>srom</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a657415e0cd3fdaa7b3d23ac128803e98</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>iptag_t</type>
      <name>tag_table</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a290212ce4c02692812b2f35ab7513bf1</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>const uchar</type>
      <name>bc_mac</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>af171d36c38fbcb714f943cee285412e2</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>const uchar</type>
      <name>zero_mac</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a60ba9867665dc144f8e0c33a0fc9b8fb</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static struct arp_entry</type>
      <name>arp_entries</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>aeada1521dcb3f509b3e5ea8413482032</anchor>
      <arglist>[MAX_ARP_ENTRIES]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint</type>
      <name>next_arp_entry</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a69aae1e59caffce47462d4b16b78e68f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>spinn_net.h</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>spinn__net_8h.html</filename>
    <class kind="struct">arp_pkt_t</class>
    <class kind="struct">udp_hdr_t</class>
    <class kind="struct">icmp_hdr_t</class>
    <class kind="struct">mac_hdr_t</class>
    <class kind="struct">ip_hdr_t</class>
    <member kind="define">
      <type>#define</type>
      <name>MAC_HDR_SIZE</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ae579a5ad7fb0ff17f20040408519595f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IP_HDR_SIZE</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>aa0808a5516425f113c81b53a6ed2f833</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UDP_HDR_SIZE</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ac46709678ac30a9f888021f6342cf735</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SDP_PAD_SIZE</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>afe0e0a590679ece550e1d2d911b2d7ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IP_HDR_OFFSET</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a5dbb1dd69f122027f6d5a02e0392b578</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IP_DATA_OFFSET</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ab0cd9060f69d166083ac2cdbbeafd32b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UDP_HDR_OFFSET</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ab6c0c6407060accff93f4212fa83bdbd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UDP_DATA_OFFSET</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ad80ecbe34882013daaf1f4cd7091ba9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETYPE_IP</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a465c490b37956eb231b04ad774bb1ecf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ETYPE_ARP</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ac9f42042236c06a678f8e66797ff9342</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PROT_ICMP</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ae8e620e2c4b1bf0461c3652238c9fccd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PROT_UDP</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>afb5ebfaff9a32a76648ad89c0ab05a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ARP_REQ</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a378c1eb5e4da70cfff3f9142a072e8f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ARP_REPLY</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a06ba7b414e718081998f2814090debf1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ICMP_ECHO_REPLY</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a5c5b1834e497f53ad0ef947bbe9777fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ICMP_ECHO_REQ</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a45cbb931aeda872385caeeb72b3bfe48</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eth_init</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ab3e1723eb1dc1c5c927c6ca06463064c</anchor>
      <arglist>(uchar *mac)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>ipsum</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>aed20e55950dc2a5b79f8cd8fc1eca42b</anchor>
      <arglist>(uchar *d, uint len, uint sum)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>copy_mac</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a527085d546d3fd3877fb49fc0e3ad8ed</anchor>
      <arglist>(const uchar *f, uchar *t)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>copy_ip</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a930b67a993941fb4d6d5554d715b4a0b</anchor>
      <arglist>(const uchar *f, uchar *t)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmp_ip</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>aa31c42aac8f8fb6fe081c3b63acf37a7</anchor>
      <arglist>(const uchar *a, const uchar *b)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>copy_ip_hdr</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a85febc90e389ed3345cde8c1d3544aed</anchor>
      <arglist>(uchar *dest, uint prot, ip_hdr_t *ip, uint len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eth_transmit</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a2a0803ce8298c4b35e566f9d0ec7ce92</anchor>
      <arglist>(uchar *buf, uint len, uint type, const uchar *dest)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eth_transmit2</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>af730bd3b9bb23eaad8975626e6dd48a3</anchor>
      <arglist>(uchar *hdr, uchar *buf, uint hdr_len, uint buf_len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>send_arp_pkt</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a1d830a83487df866b7651f8a4392ea7f</anchor>
      <arglist>(uchar *buf, const uchar *dest, const uchar *tha, const uchar *tpa, uint type)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>arp_lookup</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a334e3df9be68b50d1d05dbf146144983</anchor>
      <arglist>(iptag_t *iptag)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>arp_pkt</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a3b8c480d1e4699fdb7d5de0f2b43cac8</anchor>
      <arglist>(uchar *rx_pkt, uint rx_len, uint tag_table_size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>arp_add</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ac8f7e567b716b0e88a9e52f2364de2ce</anchor>
      <arglist>(uchar *mac, uchar *ip)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>icmp_pkt</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a6ac87dfdea22ccb270541502db9bbbc3</anchor>
      <arglist>(uchar *rx_pkt, uint rx_len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>copy_udp</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>aa9c6b4312768b7998223f802c861ff37</anchor>
      <arglist>(uchar *buf, uint len, uint dest, uint srce)</arglist>
    </member>
    <member kind="variable">
      <type>const uchar</type>
      <name>bc_mac</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>af171d36c38fbcb714f943cee285412e2</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>const uchar</type>
      <name>zero_mac</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a60ba9867665dc144f8e0c33a0fc9b8fb</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>spinn_phy.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>spinn__phy_8c.html</filename>
    <includes id="spinnaker_8h" name="spinnaker.h" local="yes" imported="no">spinnaker.h</includes>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <member kind="function">
      <type>void</type>
      <name>phy_reset</name>
      <anchorfile>spinn__phy_8c.html</anchorfile>
      <anchor>ab24afbd8a1b0c45e59ea8b1cb1fbc37b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>phy_shift_out</name>
      <anchorfile>spinn__phy_8c.html</anchorfile>
      <anchor>aa171bdd8f551054834f648f16751fdc4</anchor>
      <arglist>(uint data, uint len)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>phy_shift_in</name>
      <anchorfile>spinn__phy_8c.html</anchorfile>
      <anchor>a2f55412385ac8818f013b92b944e2044</anchor>
      <arglist>(uint len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>phy_write</name>
      <anchorfile>spinn__phy_8c.html</anchorfile>
      <anchor>a01d762287b2749d0ded71981ab369a76</anchor>
      <arglist>(uint addr, uint data)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>phy_read</name>
      <anchorfile>spinn__phy_8c.html</anchorfile>
      <anchor>a3631c41e9fb9b75c05b9e1bb4a1177a9</anchor>
      <arglist>(uint addr)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>spinn_phy.h</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>spinn__phy_8h.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>spinn_phy_registers</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_CONTROL</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfaaacc2a4679cfb3961a0dece651eeb69d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_STATUS</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfac507884139e59401704f2eba22b09eb2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_ID1</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfad786b97056a3d6030c75d12333eb0745</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_ID2</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfa4b6e9fae091b59dc0e1a1476a5ce2180</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_AUTO_ADV</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfa4842ca23d4829c6c77d243fbbc814b53</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_AUTO_LPA</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfae4dad87e43ef630ad0d39be5b02d5252</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_AUTO_EXP</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfa793954cf7e8a37d1cd23bdda5f46eaac</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_MODE_CSR</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfa03dc1d8f4ad819c31d6e14155333008b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_SP_MODE</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfa4c6740db3b133c4d12e220f0353af5c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_ERR_COUNT</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfacc3400ae1f0fecdc3fdebcdfba4d973e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_SP_CSIR</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfa6f11cca432e9206ad89888395efa2428</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_INT_SRC</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfaab79b3cf5d23ad81b210128d02a198f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_INT_MASK</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfa74fb08ef54565ef8a4737d9842022377</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PHY_SP_CSR</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ac1ea1bdadbfd2e128fcecfa9ef5b41bfa2b3ff168c0760bdebd7a19d29d35e817</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>phy_reset</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>ab24afbd8a1b0c45e59ea8b1cb1fbc37b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>phy_write</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>a01d762287b2749d0ded71981ab369a76</anchor>
      <arglist>(uint addr, uint data)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>phy_read</name>
      <anchorfile>spinn__phy_8h.html</anchorfile>
      <anchor>a3631c41e9fb9b75c05b9e1bb4a1177a9</anchor>
      <arglist>(uint addr)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>spinn_srom.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/scamp/</path>
    <filename>spinn__srom_8c.html</filename>
    <includes id="spinnaker_8h" name="spinnaker.h" local="yes" imported="no">spinnaker.h</includes>
    <includes id="sark_8h" name="sark.h" local="yes" imported="no">sark.h</includes>
    <member kind="enumeration">
      <type></type>
      <name>spinn_srom_commands</name>
      <anchorfile>spinn__srom_8c.html</anchorfile>
      <anchor>a5c620b6e871b70cbd195196e85e24c78</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>spinn_srom_scp_flag_bits</name>
      <anchorfile>spinn__srom_8c.html</anchorfile>
      <anchor>ab90d8ca950685f71aba490c1922d9eb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>ncs_low</name>
      <anchorfile>spinn__srom_8c.html</anchorfile>
      <anchor>ad2521c0a225e6ae733a45a73d97c36a8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>ncs_high</name>
      <anchorfile>spinn__srom_8c.html</anchorfile>
      <anchor>a1860ffa2daa7e4ccff34dda5377c065f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>clock</name>
      <anchorfile>spinn__srom_8c.html</anchorfile>
      <anchor>a3051385d0cf4f71eb6f0396c7d0a17cd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>send</name>
      <anchorfile>spinn__srom_8c.html</anchorfile>
      <anchor>a8146c4cc84f3cde5141509e0aab10777</anchor>
      <arglist>(uint v, uint n)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>read8</name>
      <anchorfile>spinn__srom_8c.html</anchorfile>
      <anchor>aa9778855cd6dcb21885609afdbb8c112</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>cmd_srom</name>
      <anchorfile>spinn__srom_8c.html</anchorfile>
      <anchor>a4c0ebee708bfd4951c0359ff6b869bb2</anchor>
      <arglist>(sdp_msg_t *msg)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>spin1_api.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/spin1_api/</path>
    <filename>spin1__api_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="no" imported="no">sark.h</includes>
    <includes id="spin1__api_8h" name="spin1_api.h" local="no" imported="no">spin1_api.h</includes>
    <includes id="spin1__api__params_8h" name="spin1_api_params.h" local="no" imported="no">spin1_api_params.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>IO_API</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>ab2b77f93a72e1a29fcb454002f9f5474</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>io_delay</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a8f741aeda8b3419d1810ae383d2ecdb5</anchor>
      <arglist>(us)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VIC_ENABLE_VECTOR</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>afde118e4e84c258f3b0315d6c805734a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cc_rx_error_isr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a6877e25434245843f2cb2e44dcacd5f6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cc_rx_ready_isr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>ad757a2bf64a4fd5e3447e411aa48b216</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cc_fr_ready_isr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a686b53075ec48e10b26606dc542950fe</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cc_tx_empty_isr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a816edb7651e2d7fa4efd136d76f5366b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>dma_done_isr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a31319328fd117385db84701404e49c1a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>dma_error_isr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>adcc3483511d850f58646a46735ca08ca</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer1_isr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a0bac9245b75e3987854ed1a33d36fb6b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>soft_int_isr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a197765e2b4cd5998b2b4431875a3e688</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cc_rx_ready_fiqsr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a195457f55458da4c3a67af55247dc9bd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cc_fr_ready_fiqsr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a6161acd4b7a87336319e81e2bb1b7a43</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>dma_done_fiqsr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a7efa652aed5c2b4b7ed8b27d7c8402d4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer1_fiqsr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a3fa32666819594f3e10df1eb002c08b9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>soft_int_fiqsr</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a21f0792ebbd3e885094356b9277e209b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_int_han</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a6d542339b492b2ad62a803b09b001365</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_wfi</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>ae35277172e3b42f22218009fe98a2402</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_int_enable</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a79ceeffbd0574d099d2abce815adbdec</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>configure_communications_controller</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>af00038da4eff67414469bc0dde8f84bd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>configure_dma_controller</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>ab021c9a223136d881193b2a604b89df9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>configure_timer1</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a00ef2b25b74b2f8d61c8183ef5fb12e8</anchor>
      <arglist>(uint time, uint phase)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>configure_vic</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a7b42ce9c5f12c28238c8df88fd3d72c8</anchor>
      <arglist>(uint enable_timer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_pause</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a11a9fb2774fdaf83832fa9d37752df8d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>resume</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a2bd6061935263d50bfa20b1315687968</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_resume</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a788b5c55fa3faebafb5025bb858f76c4</anchor>
      <arglist>(sync_bool sync)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>resume_wait</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>aa9f44dd3bd8b9d8ccf8e9a8924e2a194</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>dispatch</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a372fdf23579fd971462ea16885b798ea</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_callback_on</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>afc98f97be297e7077334cad216d32b57</anchor>
      <arglist>(uint event_id, callback_t cback, int priority)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_callback_off</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>aaaad2f2f52dcfbd835c3928b5db97026</anchor>
      <arglist>(uint event_id)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deschedule</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a1dd4cf99860f8d1d387e98e112254d38</anchor>
      <arglist>(uint event_id)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_get_simulation_time</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a79fba089ee6503e4c94648e0a2b3af10</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_exit</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>abe0417e22d07fa7bcdcfcc4ae29f7443</anchor>
      <arglist>(uint error)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_set_timer_tick_and_phase</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a16c5bdcb187e921a57c9b2849fd2e909</anchor>
      <arglist>(uint time, uint phase)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>clean_up</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a3e14a78f45e1471094da61125d67bc82</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>report_debug</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a5309e64859323930f3eac87f976c90e7</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>report_warns</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a33be36599ca13f2bb8a8148bb47e7ce5</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_rte</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a546b4f48774a886ed2302adf30bb4600</anchor>
      <arglist>(rte_code code)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint</type>
      <name>start</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a3c1686c281a6d3ab409c4684891399cd</anchor>
      <arglist>(sync_bool sync, uint start_paused)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_start</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>ad49e25c0f0359f6826d8addcebf1e05c</anchor>
      <arglist>(sync_bool sync)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_start_paused</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>af41b39641c2de615ac5e006cf915202f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_dma_transfer</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a1f4e5a5c1afbef1a0d3dafcfe96931a1</anchor>
      <arglist>(uint tag, void *system_address, void *tcm_address, uint direction, uint length)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_dma_flush</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a5032a57df544a2dbceb7d94cada28ffa</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_memcpy</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a8c23f19c28f2c3ed0a9b86ce8c98802d</anchor>
      <arglist>(void *dst, void const *src, uint len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_flush_rx_packet_queue</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a4703fab897e9d5409c1b569f1ace4729</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spin1_flush_tx_packet_queue</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a77921134c64d1229235f89566f060a8c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_send_packet</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a0b239ad6683a630c2926321885395e27</anchor>
      <arglist>(uint key, uint data, uint TCR)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_get_id</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a337ee4169620e7a86f8a81b81bb4f510</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_get_core_id</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>acc208cdfe1d4eb5caa298a8a5b4db50c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_get_chip_id</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>acb213e76a5a4d721b631082f30de45ad</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>schedule_sysmode</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>aae2f320075d9cf52b46f9475ec0b16cc</anchor>
      <arglist>(uchar event_id, uint arg0, uint arg1)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_schedule_callback</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a653c1de1c4181f03c7ba4dbba38df9c7</anchor>
      <arglist>(callback_t cback, uint arg0, uint arg1, uint priority)</arglist>
    </member>
    <member kind="function">
      <type>uint</type>
      <name>spin1_trigger_user_event</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a19505c72d4618b8c00646d4dff1a328f</anchor>
      <arglist>(uint arg0, uint arg1)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_pre_main</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a34ed2549430ecaaa7a6972ad7fb79434</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sark_post_main</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a752454711da73d1c9d31ccb8d1feafd2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>leadAp</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a8754c80284d361398ba776362765b92b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint</type>
      <name>run</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>ad8ba2e0758a18cd757d16c19da7248ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint</type>
      <name>paused</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a2f2721dcc1efb0f4d27e8fd9025e5f1a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile uint</type>
      <name>resume_sync</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>ab16fcbbf3fa584c2c1963e1c3114b59c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>ticks</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a7fcd6915876e066781399d7b00f1b1f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>timer_tick</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a961a184576a286ec2b70ebb01bcaddd1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>timer_tick_clocks</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a4511313ccb7768c200e73b3ff63a5d28</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>drift</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a3539afc91ff4e1a2bb2ea3ee49ab9a32</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>drift_sign</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>ae2ac308ec0c5c2302af6ef1d1c676405</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>drift_accum</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a98afce4d956b1340857dc4677531a7c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>time_to_next_drift_update</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a59613dc66e7507a614fcc590ad961e5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint</type>
      <name>timer_phase</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a9ed7cba9e92d54cbcf1c9fc4aacdfff5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static isr_t</type>
      <name>old_vector</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>ae38a4cedccf7e0c33a685a9bac26a10c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint</type>
      <name>old_select</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>ae00570bd9039bd2873bc6d127ae17b3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint</type>
      <name>old_enable</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a5f564e61a431cc2569b1ab8678a938b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>fiq_event</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a5064060c4d2ea500e8be1c05e1845eb2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>mc_pkt_prio</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>aef62729ad78f2c897ef435cd5507eee8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>fr_pkt_prio</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a4345449ff2266c63420cbb1fc5061aba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>dma_queue_t</type>
      <name>dma_queue</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a3f96945e579863d7fdc4c2110dc84555</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tx_packet_queue_t</type>
      <name>tx_packet_queue</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a96272f006bd3244775dff6460497116e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>user_event_queue_t</type>
      <name>user_event_queue</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>aea998e12bd7efbe8dfd02d4463e1748f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static task_queue_t</type>
      <name>task_queue</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>abca9b61d23a2cd43b9e57cde749f2c03</anchor>
      <arglist>[NUM_PRIORITIES-1]</arglist>
    </member>
    <member kind="variable">
      <type>cback_t</type>
      <name>callback</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a7f4271a2bb359cefd04d8ca74346ca05</anchor>
      <arglist>[NUM_EVENTS]</arglist>
    </member>
    <member kind="variable">
      <type>diagnostics_t</type>
      <name>diagnostics</name>
      <anchorfile>spin1__api_8c.html</anchorfile>
      <anchor>a4925eaa2ab3b95719d3af2c529e1d63b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>spin1_isr.c</name>
    <path>/home/travis/build/SpiNNakerManchester/spinnaker_tools/spin1_api/</path>
    <filename>spin1__isr_8c.html</filename>
    <includes id="sark_8h" name="sark.h" local="no" imported="no">sark.h</includes>
    <includes id="spin1__api_8h" name="spin1_api.h" local="no" imported="no">spin1_api.h</includes>
    <includes id="spin1__api__params_8h" name="spin1_api_params.h" local="no" imported="no">spin1_api_params.h</includes>
    <member kind="function">
      <type>void</type>
      <name>schedule</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>af15196e8b2360105ec0c061cbe9214d2</anchor>
      <arglist>(uchar event_id, uint arg0, uint arg1)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cc_rx_ready_isr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>ad757a2bf64a4fd5e3447e411aa48b216</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cc_rx_ready_fiqsr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a195457f55458da4c3a67af55247dc9bd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cc_fr_ready_isr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a686b53075ec48e10b26606dc542950fe</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cc_fr_ready_fiqsr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a6161acd4b7a87336319e81e2bb1b7a43</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cc_rx_error_isr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a6877e25434245843f2cb2e44dcacd5f6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cc_tx_empty_isr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a816edb7651e2d7fa4efd136d76f5366b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>dma_done_isr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a31319328fd117385db84701404e49c1a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>dma_done_fiqsr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a7efa652aed5c2b4b7ed8b27d7c8402d4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>dma_error_isr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>adcc3483511d850f58646a46735ca08ca</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer1_isr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a0bac9245b75e3987854ed1a33d36fb6b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>timer1_fiqsr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a3fa32666819594f3e10df1eb002c08b9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>soft_int_isr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a197765e2b4cd5998b2b4431875a3e688</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>soft_int_fiqsr</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a21f0792ebbd3e885094356b9277e209b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>ticks</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a7fcd6915876e066781399d7b00f1b1f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>timer_tick</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a961a184576a286ec2b70ebb01bcaddd1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>timer_tick_clocks</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a4511313ccb7768c200e73b3ff63a5d28</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>drift</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a3539afc91ff4e1a2bb2ea3ee49ab9a32</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>drift_accum</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a98afce4d956b1340857dc4677531a7c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>drift_sign</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>ae2ac308ec0c5c2302af6ef1d1c676405</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>time_to_next_drift_update</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a59613dc66e7507a614fcc590ad961e5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>dma_queue_t</type>
      <name>dma_queue</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a3f96945e579863d7fdc4c2110dc84555</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tx_packet_queue_t</type>
      <name>tx_packet_queue</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>a96272f006bd3244775dff6460497116e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>user_event_queue_t</type>
      <name>user_event_queue</name>
      <anchorfile>spin1__isr_8c.html</anchorfile>
      <anchor>aea998e12bd7efbe8dfd02d4463e1748f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>app_data</name>
    <filename>sark_8h.html</filename>
    <anchor>structapp__data</anchor>
    <member kind="variable">
      <type>uchar</type>
      <name>cores</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a03faf0b6de6e6c662aa1deaa1be2e96d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>clean</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac7ff7eb6b96ee4f906a21f8fdb1b43ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>sema</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a41253e9ff09514f8f67155964b3296bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>lead</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab5216a13e487731241d76e80753e91e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>mask</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a94eb883c2089bf7a129e9616600479ec</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>arp_entry</name>
    <filename>spinn__net_8c.html</filename>
    <anchor>structarp__entry</anchor>
    <member kind="variable">
      <type>uchar</type>
      <name>mac</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a33747074efe4bb7daf7c3cf1422a9492</anchor>
      <arglist>[6]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>ip</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a6eaf02a67a4a5f0d67d06c5de15c22a0</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>struct arp_entry *</type>
      <name>next</name>
      <anchorfile>spinn__net_8c.html</anchorfile>
      <anchor>a22a180d80a76d941d5ffabdf03933ff1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>arp_pkt_t</name>
    <filename>spinn__net_8h.html</filename>
    <anchor>structarp__pkt__t</anchor>
    <member kind="variable">
      <type>ushort</type>
      <name>htype</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a72c6d153c76f1f7bc5fc704ca8c534e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>ptype</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a8016e54ff3e557dbcef14d7a696f45a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>hlen</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ad2aa3f33f710748070b2c42cbc731dcd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>plen</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a7ef1ef4b3d60201218d1836b9aa0e43c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>op</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>afeaafcf6b7308ad506e2dc025226425b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>sha</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a221cdc45e9939b5403404047eafe1031</anchor>
      <arglist>[6]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>spa</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a72efd209ec478199b777f796bd76020d</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>tha</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a58730e30acfef44d4dfab623ad95986f</anchor>
      <arglist>[6]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>tpa</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a2d6e59251587f1fb7cc67175b36a93c4</anchor>
      <arglist>[4]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>block</name>
    <filename>sark_8h.html</filename>
    <anchor>structblock</anchor>
    <member kind="variable">
      <type>struct block *</type>
      <name>next</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2673a4d5a335adea413ee6d8aa394781</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct block *</type>
      <name>free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa10bfaa1967aa2428aa372f950794c51</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>cback_t</name>
    <filename>spin1__api__params_8h.html</filename>
    <anchor>structcback__t</anchor>
    <member kind="variable">
      <type>callback_t</type>
      <name>cback</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a6523738f13cfb6007f89b578780c8c6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>priority</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a7e28688ee87e7d7453a6a14f48162881</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>cmd_hdr</name>
    <filename>sark_8h.html</filename>
    <anchor>structcmd__hdr</anchor>
  </compound>
  <compound kind="struct">
    <name>copy_t</name>
    <filename>spin1__api__params_8h.html</filename>
    <anchor>structcopy__t</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>id</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>af2d72a7d68518299f947dd1594cd8087</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>tag</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a5fe417eb710ec92629eb0ea4a1702d93</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>system_address</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a7ec0f1fd63a45924cf55c8165914d6a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>tcm_address</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a2f48383847212d1821f3bca7afbdbe4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>description</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a7c1214ab279644d631228b6771af2b72</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>diagnostics_t</name>
    <filename>spin1__api_8h.html</filename>
    <anchor>structdiagnostics__t</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>exit_code</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>acd6c68c112403d79cd2696877555b9f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>warnings</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a49bb7e389464f279347cf29b8a654950</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>total_mc_packets</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a4ba16c7d900a0058226419447edd33ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>dumped_mc_packets</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a00330c81fd3e4264423ecaae945bb729</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uint</type>
      <name>discarded_mc_packets</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a1efd077066e8da7015e85fce49881968</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>dma_transfers</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a51b263b00d8c7d8d80023d41019cade4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>dma_bursts</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a4cb7060b09889eeaac7e186fb75f0ba8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>dma_queue_full</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>af6db95d506fa0ebabd265ab9c77384b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>task_queue_full</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>ae9d3d58f3edb5c2c0abcc8282704d231</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>tx_packet_queue_full</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a6403ec603f826e67843e15ecf06dbc74</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>user_event_queue_full</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a11e53d98ac6e7ea41af537a5bf27ba15</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>writeBack_errors</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a689818b3513defad5446ffbab1357872</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>total_fr_packets</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a92a7cddb2eeeb62f90004ada880f255b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>dumped_fr_packets</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a8c352bd034675275742b760bd3b98a8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>discarded_fr_packets</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>ae3a5627177650fffb7e275f55aad0a0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>in_timer_callback</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a800e895aecb0b057409e2c2935b160b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>number_timer_tic_in_queue</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a694ab93022831ae578928fa26c8d6a07</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>largest_number_of_concurrent_timer_tic_overruns</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>aa245077760802eb05d147daf90ea59ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>total_times_tick_tic_callback_overran</name>
      <anchorfile>spin1__api_8h.html</anchorfile>
      <anchor>a3a57596f4e97ee192e7f1f39352d5747</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>divmod</name>
    <filename>sark_8h.html</filename>
    <anchor>structdivmod</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>div</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3d7e3968504c52180c8210411691c145</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>mod</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac854c683365c83d375a5c61abfbcd429</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>dma_queue_t</name>
    <filename>spin1__api__params_8h.html</filename>
    <anchor>structdma__queue__t</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>start</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a780d8d9df86c83a5a04db9b8e1c9fc06</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>end</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a795d04fa9fef422007960d1760c31aae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>copy_t</type>
      <name>queue</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a0658d1c26f8b53d0e4946835eff949f6</anchor>
      <arglist>[DMA_QUEUE_SIZE]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>event</name>
    <filename>sark_8h.html</filename>
    <anchor>structevent</anchor>
    <member kind="variable">
      <type>struct event *</type>
      <name>next</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7cf9ce050dce1b674b114dfe210ce86a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>event_proc</type>
      <name>proc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aeb8e10b4164aba249e9c64069c83fe7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>arg1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0e960f9cfefac61d69fe31f11334a26d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>arg2</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a145ecc07cc2ef3e1ca74f5544267eed0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>time</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8ab837d829d4a55311e1653f40571f9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>ID</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a19d234cd0e6bc6e4e70b07e4ef0daeec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>reuse</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0d894cc386e7f06760ac7e0c5b354064</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>__PAD1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2596f3b9a84b6919c25a3af61389b05e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>__PAD2</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3411961e6834b907b971287a365ecb6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>__PAD3</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a66d4adb9d44d189342bdbd1f78ea386b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>event_data</name>
    <filename>sark_8h.html</filename>
    <anchor>structevent__data</anchor>
    <member kind="variable">
      <type>event_t *</type>
      <name>free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac6c2504ee3cbb7d3204463fd24b2a8ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>event_t *</type>
      <name>timer_queue</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8b7129ade55999fc7599413c85f24562</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>count</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4c691c6a69ffd17b5c699fdd2b53dd98</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>max</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a239a55f5843f9bf936f6b60688ee6fcb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>id</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3c8ce424fdcdf2b8266c6276f3f55f00</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>failed</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a13c3b25077ee2854b707dd52d4c4ca07</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>rc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab0f7d7ef2e232f625ba773ee1953b8ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uchar</type>
      <name>state</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7e015d97ab59684c3b6390a8d8d0e081</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>id_rsvd</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af08c059d6d4a1b8b2d74aebdbc2fca85</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>wait</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad605cbec72e2bd65462471bd7edf4f25</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>user</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a18e7d6c2a8cb792fef5ce82895db082a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>ticks</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a82f7f4878ee0a3c2e9196f756a6cdf1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>arg1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a311e1eaf863ba060cbc1b798ed0abeb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>arg2</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab420c3e743830796e0ad45ad3a15d786</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>sdp_msg_t *</type>
      <name>msg</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a083fc9240075c4c5417c2ade2e0324b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>signal</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a97e51cb680ae6378ddec7b3dae61809b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>event_proc</type>
      <name>pause_proc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0943ca9d0c0e124f1cd033693a8488ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>pause_arg2</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>addaef79d069af68f983f611b5197cc87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>exit_rc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a1e65a93a758b1525e446340cc29c1817</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>pkt_insert</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a31db1a98c176a523c049af96453820f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>pkt_remove</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8cd8fe16af8e6bb8ce434d6e2fd61333</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uchar</type>
      <name>pkt_count</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a04acf126b600a99215e9c6b733ae28f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>__PAD2</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa1d1e2404fc3734befbe738d16f7bd7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>pkt_size</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a6bef8dc8213bb3b5877e6ecd9ce00266</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>pkt_max</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>abdf98d1e9498ddf95c19c2f5f887ba3d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>pkt_t *</type>
      <name>pkt_queue</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae8e68fcabaf92c8fd529f4a9afe79115</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>vic_select</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0126f96e9e177c538d7ded8bc3db089c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>vic_enable</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aafa1504e07e960935532e9c4f8e1d8c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>old_select</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>adcff029e8c7a8493eced0f3db3ce98ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>old_enable</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a37fb15a20def9ddc0f3865413508f9d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>fiq_vector</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a383804930e9e4b0be4bdf8be9222f0f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>old_vector</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a308ff14b07804cce29a3d08c7bf1fff2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>vic_addr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a630c689141594636eddceefd5c204484</anchor>
      <arglist>[EVENT_COUNT]</arglist>
    </member>
    <member kind="variable">
      <type>proc_queue_t</type>
      <name>proc_queue</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae1ca0686d737d2c1ad3288815d224305</anchor>
      <arglist>[PRIO_MAX+1]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>event_vec</name>
    <filename>sark_8h.html</filename>
    <anchor>structevent__vec</anchor>
    <member kind="variable">
      <type>ushort</type>
      <name>proc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a047780772465d01d468a7e2cf31ff567</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>slot</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af3fa0eb0ec712fef68cd9f1e1dbd8bd9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>priority</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5aaa6e8361298a1d104801096e742628</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>heap_t</name>
    <filename>sark_8h.html</filename>
    <anchor>structheap__t</anchor>
    <member kind="variable">
      <type>block_t *</type>
      <name>free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af069ef1a7dcdf0052e3670d96c66d12a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>block_t *</type>
      <name>first</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af266f3069241fd88f74e9417ee4b1b19</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>block_t *</type>
      <name>last</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3ac677fcc41d0791ef656b94e9038ba6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>free_bytes</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a1d4afb841969eacd1a9e154dd552eda0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>buffer</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a20d2f0218feb16fbe0b301dfec56fe49</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>icmp_hdr_t</name>
    <filename>spinn__net_8h.html</filename>
    <anchor>structicmp__hdr__t</anchor>
    <member kind="variable">
      <type>uchar</type>
      <name>type</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>afd7300ec35806c2285aaa95f1c3e0b23</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>code</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ad7acf186adff769b803e13529a87982d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>checksum</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a6de00ae60c861d8d1eadab6007a328da</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>ident</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a7a4e2fef0671669f5f512a172dffa283</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>seq</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a445642f6d42e19a57b283458c3254581</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>iobuf</name>
    <filename>sark__io_8c.html</filename>
    <anchor>structiobuf</anchor>
  </compound>
  <compound kind="struct">
    <name>ip_hdr_t</name>
    <filename>spinn__net_8h.html</filename>
    <anchor>structip__hdr__t</anchor>
    <member kind="variable">
      <type>uchar</type>
      <name>ver_len</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a6aa7146b87142cf5726e8c0b9ec11f6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>DS</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a16ed05e077e3c1f75a258903d8b05aa4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>length</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a20cc555bb4205fef5ca7af459f10219e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>ident</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>af97976005bb4db75d4d79484ee033437</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>flg_off</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a24143125bfa198da77c9a754dd717c5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>TTL</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a40a96dd9f4e723ea97f138800513a126</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>protocol</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a7f6502f0a8a47c94afed3d20e3f4d48c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>checksum</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ad054031fc70fca1bed1f52c79f0ba5f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>srce</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a5b81e94efd54fec0a5dd48f2625b193f</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>dest</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>af626534329127da531efb272c78f5c16</anchor>
      <arglist>[4]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>iptag_t</name>
    <filename>scamp_8h.html</filename>
    <anchor>structiptag__t</anchor>
    <member kind="variable">
      <type>uchar</type>
      <name>ip</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a8f60e7faa09bcd04effd71eab478bf85</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>mac</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6b869362bb4857c40b62a62c88bdc69d</anchor>
      <arglist>[6]</arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>tx_port</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a1c93f2fde60ff199b54537f815b31aa5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>timeout</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a48800db80ec259b5d9b9315879a894fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>flags</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ac923398da016d4b90217761532febae0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>count</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>afa0f4ec60fc8ddd98e5dd0c5c8bd01e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>rx_port</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a7e426860a93eb6e6ac9a97038215552b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>dest_addr</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ad448d846ba474ec77ac880ed69dc4ba2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>dest_port</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a6bee187233d490bc6bdb261a9025ba51</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>level_t</name>
    <filename>scamp_8h.html</filename>
    <anchor>structlevel__t</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>level_addr</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aa823cf654c542ba8df4ca2ee6eab4c07</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>sent</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a758bf06a79674142fede59c696649fbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>rcvd</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aaa914dcd40a087110282b3b37478a252</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>parent</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a957372fa67250a0ea787d4898d009055</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>result</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ac2ddca5e48d8e327852f9e132e214a70</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>addr</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ac4749ffdfc51aadbec0599360ea9d4b0</anchor>
      <arglist>[16]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>valid</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>acde55018e3535977accd3c9baee377ef</anchor>
      <arglist>[16]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>mac_hdr_t</name>
    <filename>spinn__net_8h.html</filename>
    <anchor>structmac__hdr__t</anchor>
    <member kind="variable">
      <type>uchar</type>
      <name>dest</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>afc4c89acd4f3a94fab2129615ff4e457</anchor>
      <arglist>[6]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>srce</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a4896eda48cfcf3ef733c712a3c1eefa2</anchor>
      <arglist>[6]</arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>type</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>acdffe5653ee95a01e5c15b0ab1886f91</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>mem_block</name>
    <filename>sark_8h.html</filename>
    <anchor>structmem__block</anchor>
    <member kind="variable">
      <type>mem_link_t *</type>
      <name>free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2179f9434fa237445e92febd1292e045</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>count</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac8b0383560afd0a6889d330ab9e181c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>max</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3bc3861dd1ca4a9b825c8cb5af612071</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>mem_link</name>
    <filename>sark_8h.html</filename>
    <anchor>structmem__link</anchor>
    <member kind="variable">
      <type>struct mem_link *</type>
      <name>next</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0b90e6b837df3dd0afab6221a87963cc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>nn_desc_t</name>
    <filename>scamp-nn_8c.html</filename>
    <anchor>structnn__desc__t</anchor>
    <member kind="variable">
      <type>uchar</type>
      <name>state</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ac2a3efcb66319a4334447d557ed71a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>error</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a4c4087ceeea97f8ec5e326f938650abf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>forward</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a5ca95eceaa7aff2dd030cae865448b7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>retry</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ab78d04c7024a9ee6aeb833caaf4f3168</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>id</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>afa6bd34ab32c14a2e280340c67317e70</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>block_len</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a21b71c178084b7ac68093a024f15fe69</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>block_count</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ac0a94c707e2fc033eb79b021d1a3bd4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>block_num</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a6572a4a63ec29f0f3302f55667ff176f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>app_id</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a8e08a66559819ad679346243e7e8eb09</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>load</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a2952d5e0a0b026163c04c2a3d0c3932f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>srce_addr</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a759186b554df2a6e13edb370f3f17ac2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>word_len</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a9ad07d17db5e08f888b85b1ea5d2abdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>word_count</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a165a654f9ee80e4914812059eb3fb0b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>gidr_id</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ab0220b89bc365b6e1ef669bb54e06833</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>sum</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a5a343f6659fd283b1da1e195d270f826</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>region</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>af608f29a0913f9662e9d4b16b36672e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>cores</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a8d54cf0d6bc04fdad9828ed043d98a03</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>aplx_addr</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a7be42a0ca538854830340a96a1b0b99a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>load_addr</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a51c55d2237da0ce273a70bf7695cec6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>id_set</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a0178fa7365a1cf2256d44ceedaa71b86</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>biff_id_set</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ab082317572856bf1d509d559034942af</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>fbs_set</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a9a31310d34002ce87e63ed1236b37a3c</anchor>
      <arglist>[8]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>fbd_set</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a961471acbce21e895f2eb312ce505c19</anchor>
      <arglist>[8]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>fbe_set</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a427f0676bd97497223166fbd9d0fbdf2</anchor>
      <arglist>[8]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>stats</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ad045a6a8ae64d48c194fe41699ed7679</anchor>
      <arglist>[16]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>errors</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>aaf8f96a157194484df1f963b2049f275</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>buf</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>a7d1b9601d7c9921d611b7eaf920b23ea</anchor>
      <arglist>[SDP_BUF_SIZE/4]</arglist>
    </member>
    <member kind="variable">
      <type>uint64</type>
      <name>last_ffcs</name>
      <anchorfile>scamp-nn_8c.html</anchorfile>
      <anchor>ab8a40817abe2179022891abcf3a9119e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>packet_t</name>
    <filename>spin1__api__params_8h.html</filename>
    <anchor>structpacket__t</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>key</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>ac9510e2ead42de6b4e91cf6100516012</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>data</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a8f527e4d2626d8a5f25ff911649b9c3d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>TCR</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a15833e60c2cd8fc00fe01415d0d44d08</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>pkt</name>
    <filename>sark_8h.html</filename>
    <anchor>structpkt</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>ctrl</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a446942c9c5cd133d4caca96ff677619f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>data</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7da908a13046e9e2ca3a7b8ec71e5689</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>key</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad395c9882ed696efd538325cd2969e11</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>pkt_buf_t</name>
    <filename>scamp_8h.html</filename>
    <anchor>structpkt__buf__t</anchor>
    <member kind="variable">
      <type>struct pkt_buf_t *</type>
      <name>next</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a46f9e50430833f3aa536b8c65f79fb3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uchar</type>
      <name>flags</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>abb09fc97bb7743298f7e8d9afda851bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>fwd</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a2263dbf8368bca7c9e1e7932da84bb86</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>delay</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a369859ddffab1960bd140774e0d6ef3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>link</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>aba72d9ae41e95dd6b7796caa9b470cac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>pkt_t</type>
      <name>pkt</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a14bea6bd55ca839775b1988c8d39e358</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>pkt_queue_t</name>
    <filename>scamp_8h.html</filename>
    <anchor>structpkt__queue__t</anchor>
    <member kind="variable">
      <type>uchar</type>
      <name>insert</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>ae6f509356f9157c14577b74ae6e747f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>remove</name>
      <anchorfile>scamp_8h.html</anchorfile>
      <anchor>a922e0e5aba05036e7845ee7039f5612f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>proc_queue</name>
    <filename>sark_8h.html</filename>
    <anchor>structproc__queue</anchor>
    <member kind="variable">
      <type>event_t *</type>
      <name>proc_head</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>adeda8d6855877bedcb9e27b68655b37d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>event_t *</type>
      <name>proc_tail</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5f6ac916a531ea369912496e2e7b1f2a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>rtr_entry</name>
    <filename>sark_8h.html</filename>
    <anchor>structrtr__entry</anchor>
    <member kind="variable">
      <type>ushort</type>
      <name>next</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a837b192241d448b82a0e22fc7dbe1e89</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad3442f520453238fa20c3c6173bed207</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>route</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a296b14c71ae70f1bbc9bbd8d3da380b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>key</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a9c5508ee71a3cdd99d4dfcad82241e86</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>mask</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac01e32bc506d3ac346b123e5e264b223</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>rx_desc</name>
    <filename>scamp-p2p_8c.html</filename>
    <anchor>structrx__desc</anchor>
    <member kind="variable">
      <type>event_t *</type>
      <name>event</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a007e97ff8759a84a454ea8eee8199437</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>sark_data</name>
    <filename>sark_8h.html</filename>
    <anchor>structsark__data</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>virt_cpu</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4f87d828e26f0cc355328c03fce13035</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>phys_cpu</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a43e4585516e81413d3f92acc9e29c0d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>random</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af3f3c4796479c02d2300bc7728f21621</anchor>
      <arglist>[2]</arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>stack_base</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac644ffbe265ae45997e7b06e34df9218</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>heap_base</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac7d435d8164da80f9ae4513d03ce665d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>heap_t *</type>
      <name>heap</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad1154cc0be192923b9d5ee3672ac6bd8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>vcpu_t *</type>
      <name>vcpu</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7dd6ac4cf6ad94b7d8aab41ad17e5f3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>mem_block_t</type>
      <name>msg_root</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae9bd398fb6a8e8df80841dd669956f51</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>msg_rcvd</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a776b384c4f27dcc6bca7809a3d5a3438</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>msg_sent</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0f7d858938f16a8be0552cbfdd646003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>cpu_clk</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afdaf4ab69177e66f5223b6a8a36ec2aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>sw_rte</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a45a7836880978f08a4c7628648ba2a3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>__PAD1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a12f4ba2cc848d2910952dff754c470de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>sdram_buf</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a508db8b4e58f7bdf4a6ec5cc8a09dadb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>pkt_count</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a779e52585d8175137d8e270390eef228</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>sark_vec</name>
    <filename>sark_8h.html</filename>
    <anchor>structsark__vec</anchor>
    <member kind="variable">
      <type>int_handler</type>
      <name>reset_vec</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a957a4bb84b7cee6fc6c5b3939e6cb26b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>undef_vec</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a144656d1732c8ec4a1107c2febc1fa1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>svc_vec</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aba2f4eb7330cda55a1f4e2f32ab48ee6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>pabt_vec</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>abb917fd4071d94cc22f89f384f3333fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>dabt_vec</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>abcb0b9a2a97682787a13ad4b2d0bf79a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>aplx_proc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac7b289074f19606044463d26ff0f848a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>irq_vec</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a118f7fb61fced443cce311fa9e6ec2bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int_handler</type>
      <name>fiq_vec</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa177a7ca9d24d9432e155c0b3b26389c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>svc_stack</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2cb37674de5ac06a308a8865ae49de48</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>irq_stack</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a087dfc694f2fd70c85b12719afbd9ba0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>fiq_stack</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4c5bb032468d2a711c62b13f2bb00dd6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>stack_size</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a9e0c2ffd39f342e099371d5eff5c2e8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>code_top</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a067e742acbdb246c2625dd44cab1d80d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>heap_base</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a455e9828ded585e8a8a4b0ddebc229b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>stack_top</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac584f87e7139f445a703e7e7e6924bf0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>stack_fill</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a760b5d9f57eee716499ffca250cb8ea9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>num_msgs</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afddb887193e2fcd6e99144ed25eff5f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>sark_slot</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a169ffc75aeda9f0a2576d63bda6ba03d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>num_events</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8fd083609e41c6c01990e62a05a407bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>api</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a317a33e57453999b40501074edf60950</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>app_id</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af7ef25ea91a26ce6ce03cbf103103e5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uchar</type>
      <name>app_flags</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8b8b5db6b4be6a6e9a52d7fab8852e98</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>__PAD</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2e19d9364a3607893cb56a5492e54ca4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>event_vec_t</type>
      <name>event</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae59c27c01d060aa7ad4194670d6a1277</anchor>
      <arglist>[EVENT_COUNT]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>sdp_hdr</name>
    <filename>sark_8h.html</filename>
    <anchor>structsdp__hdr</anchor>
  </compound>
  <compound kind="struct">
    <name>sdp_msg</name>
    <filename>sark_8h.html</filename>
    <anchor>structsdp__msg</anchor>
    <member kind="variable">
      <type>struct sdp_msg *</type>
      <name>next</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa4dc1e4782704a46693acdf14d41541d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>length</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a96930ca762f944ab83021cc9b1b6d224</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>checksum</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a82c5da2b481d412d8ab253e21dacd21e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>flags</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab9a87e30ca1882f149a3f0ec18a98c17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>tag</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>adb9a695bdf988f48455b77bf5cb82087</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>dest_port</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a16a3c7eff91d4a75e3152a1b732d7223</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>srce_port</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7fab4945d3f2ad6b1c229f52feac7367</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>dest_addr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af8a2d4c43158d574b4af13eab3efabee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>srce_addr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a23b5b30f9c84ee82a95c4519d0b77f76</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>cmd_rc</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a28ab319a67bc3fe646dfa1687b2d55dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>seq</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4e0870d6f54952fa3c4a5099ad60c5cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>arg1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2880bf80ff7dba715edf0288a5e05d55</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>arg2</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>acab30dc3c344740f39052fb8191f2376</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>arg3</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a493df728efaa784366278f71e2c99926</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>data</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a18357e4e2c730076c5d88ab11e925d5f</anchor>
      <arglist>[SDP_BUF_SIZE]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>__PAD1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0a987c8b8009e46164437fac1c65290d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>srom_data</name>
    <filename>sark_8h.html</filename>
    <anchor>structsrom__data</anchor>
    <member kind="variable">
      <type>ushort</type>
      <name>flags</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afe5194ab8e8f8281cb7d5d0b8a3d7f1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>mac_addr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab28064e46bc1d2971f21092ed4137694</anchor>
      <arglist>[6]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>ip_addr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a79c708badc16869cd3a366dbd3c33209</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>gw_addr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae1bc2b1ef29d382549a3210bf998d7f7</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>net_mask</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae9fddb264a83151068336074926a24bf</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>udp_port</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a9af5a7e4f460fd1c35798062643bd0c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>__PAD1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a64c793fb5f105054241f9691ad233336</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>__PAD2</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5ad1761bb9ddf4b0402999ac11b2678e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>__PAD3</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a33317ee9f11de4b70e10870bcfa1bff5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>sv</name>
    <filename>sark_8h.html</filename>
    <anchor>structsv</anchor>
    <member kind="variable">
      <type>ushort</type>
      <name>p2p_addr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3ef30ef8b8fa423cc19032c4d403af82</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>p2p_dims</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a993686986de3eebef3e7463159055c84</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>dbg_addr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a67c168b6170d6dd84520356395f07c19</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>p2p_up</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad0047164286039aa2cb6cfa7c07f32b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>last_id</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a86b823aab34f13fc449d1119aab78eac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>eth_addr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afa9425c2cae1cd8937f84963ffe960bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>hw_ver</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad851f632a988748aa6a80fed182e2395</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>eth_up</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>addb12ded24ea36116d160b87b4ef8c0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>p2pb_repeats</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afa89e4421501b6f60a4788c7e54ee47c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>p2p_sql</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8733e7739140b4c04017ee0fcac6a527</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>clk_div</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad62885f0d06097f37ef08a60fde70fba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>tp_scale</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a2f80b9081f836baf561171b8031a87d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uint64</type>
      <name>clock_ms</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a59ca2a1f62e11482c51daa0dd6bcefc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile ushort</type>
      <name>time_ms</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac45a35193fe1a8285e5b5d9b3fdc6db5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>ltpc_period</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a418eb32955c44913a7e3ab32ac93cc1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uint</type>
      <name>unix_time</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8145163b1034a8f183abf180bc4c15b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>tp_timer</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8738462aa59b0cad94870205518fc73e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>cpu_clk</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a36cd7460c04619ce59d0ecae7edb061f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>mem_clk</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a165c92fc4ee05ad40c59780172a1dbec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>forward</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8494d1f8b5eeee4c81a4e09ac753a955</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>retry</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a69a416e5999c839388dadfde30b5c8a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>peek_time</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4b593e727280aa4e029745b51884cda3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>led_period</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a207810d195e2acfa6dcb787b6377840f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>netinit_bc_wait</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa46f1e4f615b91ec072592d8f36e3c24</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>netinit_phase</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8d2c67bb5ca4feb7aa0962faf2aee068</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>p2p_root</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a28418f008d48b41b98aaed4f62b38d80</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>led0</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a63d0c61f41077b6afa3364bc34e2c223</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>led1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab539b50b5d7b94572235581eb3016fa0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>clock_drift</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0820424d4a8a94eb35f82b3f0b5fd8f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>random</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab510344b65d109408def24853ae6b62f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>root_chip</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a52c340455ae61a1598a44c735948bde5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>num_buf</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af081c6b2cea9faa327391ff3c2888d7c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>boot_delay</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aed0222c65d35137347d06d02cc49a13a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>soft_wdog</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac06fa8c401f8d6b278b4eced0270330d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>sync_alignment</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a1181f25e88c40c0ff043b714256fb93c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>heap_t *</type>
      <name>sysram_heap</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a1b00ebdd6402d9ad0cf193ba303e5a62</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>heap_t *</type>
      <name>sdram_heap</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7b107b745e1535921e1b6beafff97a67</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>iobuf_size</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a7fda3f3bcf6095bc28bdf998275dc68c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>sdram_bufs</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a8bb1090cd0a266586fd68cddbacffcee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>sysbuf_size</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>add66181e9e182756d1f741b2eba19b2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>boot_sig</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aac8c222de5583dbee2f7857ae020f0f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>mem_ptr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a949d58f2cc20753cb9598b335ce48033</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uchar</type>
      <name>lock</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa18e2811bd922fb04284e5a7ce41e607</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>link_en</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ac0b877894725b5c10170f41d8817e360</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>last_biff_id</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a33d6bb698cac5a8aea5a12b2e4fe635b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>bt_flags</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>addc3a3a828f184132ecf2bbb566574d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>mem_block_t</type>
      <name>shm_root</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af6c651fa8a91cfaba42c570c375cac54</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>utmp0</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0c8f7f3a5dd2e0c0849fab9c2b96aae2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>utmp1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a12817e1d225e91a98018d8f6722b2ee7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>utmp2</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a25b658e55d291af39cdf0b7107600428</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>utmp3</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a6dfeb178325bba48d19ab6a27b9daf47</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>status_map</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aafefa913c80dd2b5420fcac6dee1dcdd</anchor>
      <arglist>[20]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>p2v_map</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0093d5cd3ea89ae4f6412918fe60779c</anchor>
      <arglist>[20]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>v2p_map</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab618ab52b71075c69f6dc38d9955cea0</anchor>
      <arglist>[20]</arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>num_cpus</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af63f89b34be67f6bb5a2d43bfff771b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>rom_cpus</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a598866290f14afb88f0697107931bda7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>board_addr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>af343aec53ee1c074509f121399db57f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>sdram_base</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aafc01e5793913e043394bb2bae816412</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>sysram_base</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a09c43940151d8ce3ebf6f2fd18efa43b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>sdram_sys</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a98e68a13af2b525452f79156f6c185b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>vcpu_t *</type>
      <name>vcpu_base</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa4937617c565f21a8cd51fd63fb76cc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>heap_t *</type>
      <name>sys_heap</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>acae31aa99b3c6e5c4cc48f4500c4db5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>rtr_entry_t *</type>
      <name>rtr_copy</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a1209fd492e9560d3b7551a42f0cfd978</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>hop_table</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aede176bcc04ee573de8dc6a4763a9893</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>block_t **</type>
      <name>alloc_tag</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0441b9a222d748aed8100e5e1e963002</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>rtr_free</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab5f22ae799cbcdac377314245eed429f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>p2p_active</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a81fe348582538a1d7189af0c2fb17e7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>app_data_t *</type>
      <name>app_data</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afaaf9feeae439e90ee3f134161097e50</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>sdp_msg_t *</type>
      <name>shm_buf</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afa7473dcda72504864708fcc8b6afd6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>mbox_flags</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad11dd91ee3fc3b8bad8b94564ffbc65f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>ip_addr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ad81bd9dbddee76ee90c343881905cf18</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>fr_copy</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a76a0315cfa702b9c3b96f5b68d0cc004</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint *</type>
      <name>board_info</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ab46b1da3b9c6883d3abecef8585ce77a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>sw_ver</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a359b23fc050904ee86c18d41272aa8f0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>task_queue_t</name>
    <filename>spin1__api__params_8h.html</filename>
    <anchor>structtask__queue__t</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>start</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a7cc9707f2456c12d03a5d2e4f9542c20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>end</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a65af69b9a59d2ff98b87cc255e644006</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>task_t</type>
      <name>queue</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a6f9ab3953a7cd479a02e0e905b4d1e54</anchor>
      <arglist>[TASK_QUEUE_SIZE]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>task_t</name>
    <filename>spin1__api__params_8h.html</filename>
    <anchor>structtask__t</anchor>
    <member kind="variable">
      <type>callback_t</type>
      <name>cback</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a0aaab6b359c7996f220e82d7548e781d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>arg0</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a4123b8c7a65507e1c39774a2f260cce1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>arg1</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a3bce18edbfcdf02639c23e74a805fec4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tx_desc</name>
    <filename>scamp-p2p_8c.html</filename>
    <anchor>structtx__desc</anchor>
    <member kind="variable">
      <type>event_t *</type>
      <name>event</name>
      <anchorfile>scamp-p2p_8c.html</anchorfile>
      <anchor>a12f1a37322d99adfdd701ca386d6948c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tx_packet_queue_t</name>
    <filename>spin1__api__params_8h.html</filename>
    <anchor>structtx__packet__queue__t</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>start</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>ad9d49648a6b77a4b6f55a280678adcab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>end</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a0fdff041f07e2aacdec6eba513fb4c4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>packet_t</type>
      <name>queue</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>af17cfbcbf1009f244335c1520f3ca7ad</anchor>
      <arglist>[TX_PACKET_QUEUE_SIZE]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>udp_hdr_t</name>
    <filename>spinn__net_8h.html</filename>
    <anchor>structudp__hdr__t</anchor>
    <member kind="variable">
      <type>ushort</type>
      <name>srce</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ae63a3d477b46d2544d417b6b612fd8c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>dest</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>a8f398a3055c97c98f3be70ff6c44e267</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>length</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>af0337157bc324b9aeccf65dc5c9de3d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>checksum</name>
      <anchorfile>spinn__net_8h.html</anchorfile>
      <anchor>ae05849b8f690bba249b0d2f6dbc48486</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>user_event_queue_t</name>
    <filename>spin1__api__params_8h.html</filename>
    <anchor>structuser__event__queue__t</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>start</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a1fc6717d3a3b07cfe5517bc02c0b7e36</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>end</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a8890ecc525a400ddc91faec852855675</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>user_event_t</type>
      <name>queue</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a1149c3de1263152672dc006e09203420</anchor>
      <arglist>[USER_EVENT_QUEUE_SIZE]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>user_event_t</name>
    <filename>spin1__api__params_8h.html</filename>
    <anchor>structuser__event__t</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>arg0</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>a183b2a3834cf8a853f1e34939c115ad6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>arg1</name>
      <anchorfile>spin1__api__params_8h.html</anchorfile>
      <anchor>af853f0eda34b6df4b2ff8ae5d6de362b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>vcpu</name>
    <filename>sark_8h.html</filename>
    <anchor>structvcpu</anchor>
    <member kind="variable">
      <type>uint</type>
      <name>r</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a97e729cd2f3d945954f3c4941bbbb09d</anchor>
      <arglist>[8]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>psr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0483f6a431a28481b6809d5ef244e957</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>sp</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a88313fabb0b571331ac645dd3a12a4e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>lr</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a09e437419d72539a2de00c194db8dc44</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>rt_code</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a55f091be035b47d0cf44894438c47040</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>phys_cpu</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>acfbefb397a03ae169bd144d3994f1efe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>cpu_state</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a0fd4898b354e0a5f2de6d482af607ff3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uchar</type>
      <name>app_id</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa93d64ace2d7183c32fdf5dc8a9227bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>mbox_ap_msg</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a15614bd87301ba1b0ad368c9c6882937</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>mbox_mp_msg</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>afc12f9cefbfcc624aeb4239c6532526f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uchar</type>
      <name>mbox_ap_cmd</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>abd7a8dd012f6cc05c0726f9b7037a99a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uchar</type>
      <name>mbox_mp_cmd</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a6ace8816c90df975596bdc4f516b720b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushort</type>
      <name>sw_count</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a5878f656e58d46ad86ad527fb59115ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>sw_file</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a6419a5ffc0cad6365a1703b70bcd6aab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>sw_line</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a9731df68f9b996c84c8817f159bbdab6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>time</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a87ceff7363ab84a0b2cb7d9160512525</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>app_name</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa6cdc553a78629baf29895d2759bd92b</anchor>
      <arglist>[16]</arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>iobuf</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a20bb1d3aa3f07eb13989c30b2586199d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>sw_ver</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>aa560cda0875de9d3f0f6ec964ce7aeaa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>__PAD</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a77179940777fe7360b4abd09ca520db9</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>user0</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a3dffcf3dab335fc4c0a84acfd4b502bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>user1</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a56cd6d66d640742bc6d18bdc1764377f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>user2</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>ae20cca30ebae3ea9287230334eeabb66</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint</type>
      <name>user3</name>
      <anchorfile>sark_8h.html</anchorfile>
      <anchor>a4f6cb948e8edb36f376f4981b2fff632</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>spinnaker_tools: SpiNNaker API, SARK, SC&amp;MP, and Spin1 API</title>
    <filename>index</filename>
  </compound>
</tagfile>
