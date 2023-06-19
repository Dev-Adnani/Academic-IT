const AddRecord = ((name, designation) => {
  let _name = name;
  let _designation = designation;

  const updateName = (newName) => {
    _name = newName;
  };

  const updateDesignation = (newDesignation) => {
    _designation = newDesignation;
  };

  return {
    updateName,
    updateDesignation
  };
})("Dev Adnani", "CEO");