EJSON_KEYDIR ?= /opt/ejson/keys

all: password.json password.h

password.json:
	@ejson encrypt $@

password.h: password.h.tmpl password.json
	@gomplate --context=.=password.json -f password.h.tmpl -o $@

.PHONY: all password.json
.DELETE_ON_ERROR:
.SECONDARY:
