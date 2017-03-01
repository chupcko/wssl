.DEFAULT_GOAL := all
.PHONY: all
all: source-

.PHONY: debug
debug: source-debug

.PHONY: clean
clean: source-clean
	rm -rf out

define GENERATE_GOALS_CORE =
  GOALS += $(1)-$(2:all=)
endef
define GENERATE_GOALS =
  $(foreach goal,all debug clean,$(eval $(call GENERATE_GOALS_CORE,$(1),$(goal))))
endef
$(foreach dir,source,$(call GENERATE_GOALS,$(dir)))

GET_DIR = $(word 1,$(subst -, ,$(1)))
GET_GOAL = $(word 2,$(subst -, ,$(1)))

.PHONY: $(GOALS)
$(GOALS):
	$(MAKE) -C $(call GET_DIR,$@) $(call GET_GOAL,$@)
