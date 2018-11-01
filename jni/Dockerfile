ADD file:bcd068f67af2788dbd57729c0c8193f022ec5c37fefb8704390c59081152e6fc in /

RUN set -xe   && \
	echo '#!/bin/sh' > /usr/sbin/policy-rc.d  && \
	echo 'exit 101' >> /usr/sbin/policy-rc.d  && \
	chmod +x /usr/sbin/policy-rc.d   && \
	dpkg-divert --local --rename --add /sbin/initctl  && \
	cp -a /usr/sbin/policy-rc.d /sbin/initctl  && \
	sed -i 's/^exit.*/exit 0/' /sbin/initctl   && \
	echo 'force-unsafe-io' > /etc/dpkg/dpkg.cfg.d/docker-apt-speedup   && \
	echo 'DPkg::Post-Invoke { "rm -f /var/cache/apt/archives/*.deb /var/cache/apt/archives/partial/*.deb /var/cache/apt/*.bin || true"; };' > /etc/apt/apt.conf.d/docker-clean  && echo 'APT::Update::Post-Invoke { "rm -f /var/cache/apt/archives/*.deb /var/cache/apt/archives/partial/*.deb /var/cache/apt/*.bin || true"; };' >> /etc/apt/apt.conf.d/docker-clean  && echo 'Dir::Cache::pkgcache ""; Dir::Cache::srcpkgcache "";' >> /etc/apt/apt.conf.d/docker-clean   && echo 'Acquire::Languages "none";' > /etc/apt/apt.conf.d/docker-no-languages   && \
	echo 'Acquire::GzipIndexes "true"; Acquire::CompressionTypes::Order:: "gz";' > /etc/apt/apt.conf.d/docker-gzip-indexes   && \
	echo 'Apt::AutoRemove::SuggestsImportant "false";' > /etc/apt/apt.conf.d/docker-autoremove-suggests

RUN rm -rf /var/lib/apt/lists/*
RUN mkdir -p /run/systemd && echo 'docker' > /run/systemd/container

CMD ["/bin/bash"]

RUN apt-get update && apt-get install -y default-jre socat curl
RUN useradd -m pwn

COPY file:08510fc60a932ab77b50301b65162c89c9d88956460a5ed66fc3c4549ccf7f2c in /home/pwn/
COPY file:ba0379a03977726c67163e9324fcb0494e43c57923a30cc8229bedc725225594 in /home/pwn/
COPY file:92de2b811bb86caafdd8f6c966533a447ed7b3e57d7921c0505bfc905b401f7f in /send_flag

WORKDIR /home/pwn/

CMD ["/bin/sh" "-c" "LD_LIBRARY_PATH=. socat TCP-LISTEN:4444,reuseaddr EXEC:'java -Xms32M -Xmx500M CbJava'"]
