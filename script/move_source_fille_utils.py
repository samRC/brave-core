# Copyright (c) 2022 The Brave Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/. */

import os

BRAVE_DIR_NAME = 'brave'

def is_in_brave_dir():
  _, tail = os.path.split(os.getcwd())
  return tail == BRAVE_DIR_NAME


def to_src_relative_path(dir):
  if is_in_brave_dir():
    return os.path.join(BRAVE_DIR_NAME, dir)
  return dir


def to_cwd_relative_path(dir):
  if is_in_brave_dir():
    return os.path.normpath(os.path.join(os.pardir, dir))
  return dir
