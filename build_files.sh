build_task() {
  local task_dir=$1
  echo "Currently building: $task_dir"

  cd "$task_dir" || { echo "$task_dir is missing"; exit 1; }

  qmake || { echo "qmake error"; exit 1; }
  make || { echo "make error"; exit 1; }

  cd - || exit 1
}

build_task "$HOME/dbus-test-task/firstTask/"
build_task "$HOME/dbus-test-task/secondTask/"
build_task "$HOME/dbus-test-task/thirdTask/"

echo "Building completed"
