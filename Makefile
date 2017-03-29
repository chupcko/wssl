.DEFAULT_GOAL := all
.PHONY: all
all: source- examples- demonstrations-

.PHONY: debug
debug: clean source-debug examples-debug demonstrations-debug

.PHONY: clean
clean: source-clean examples-clean demonstrations-clean
	rm -r -f out core

define GENERATE_GOALS_CORE =
  GOALS += $(1)-$(2:all=)
endef
define GENERATE_GOALS =
  $(foreach goal,all debug clean,$(eval $(call GENERATE_GOALS_CORE,$(1),$(goal))))
endef
$(foreach dir,source examples demonstrations,$(call GENERATE_GOALS,$(dir)))

define GENERATE_RULES_CORE =
  $(1)-$(2:all=): source-$(2:all=)
endef
define GENERATE_RULES =
  $(foreach goal,all debug,$(eval $(call GENERATE_RULES_CORE,$(1),$(goal))))
endef
$(foreach dir,examples demonstrations,$(call GENERATE_RULES,$(dir)))

GET_DIR = $(word 1,$(subst -, ,$(1)))
GET_GOAL = $(word 2,$(subst -, ,$(1)))

.PHONY: $(GOALS)
$(GOALS):
	$(MAKE) -C $(call GET_DIR,$@) $(call GET_GOAL,$@)
